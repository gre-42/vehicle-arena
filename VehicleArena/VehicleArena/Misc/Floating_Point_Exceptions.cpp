// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Floating_Point_Exceptions.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <stdexcept>

#ifdef _WIN32

#include <float.h>
#include <stdexcept>
#include <iostream>

using namespace VA;

// #ifdef _MSC_VER
// #pragma float_control(except, on)
// #endif

void VA::enable_floating_point_exceptions() {
    unsigned int control_word;
    {
        errno_t err = _controlfp_s(&control_word, 0, 0);
        if (err != 0) {
            throw std::runtime_error("Could not get float point control word");
        }
    }
    // From: https://stackoverflow.com/questions/4282217/visual-c-weird-behavior-after-enabling-floating-point-exceptions-compiler-b
    control_word &= ~(unsigned int)_EM_INVALID;
    {
        unsigned int control_word2 = 0;
        errno_t err = _controlfp_s(&control_word2, control_word, _MCW_EM);
        if (err != 0) {
            throw std::runtime_error("Could not set float point control word");
        }
    }
}

TemporarilyIgnoreFloatingPointExeptions::TemporarilyIgnoreFloatingPointExeptions()
{
    {
        errno_t err = _controlfp_s(&control_word_, 0, 0);
        if (err != 0) {
            throw std::runtime_error("Could not read control word");
        }
    }
    {
        unsigned int control_word = 0;
        errno_t err = _controlfp_s(&control_word, _CW_DEFAULT, _MCW_EM);
        if (err != 0) {
            throw std::runtime_error("Could not restore default control word");
        }
    }
}

TemporarilyIgnoreFloatingPointExeptions::~TemporarilyIgnoreFloatingPointExeptions() {
    unsigned int control_word2 = 0;
    errno_t err = _controlfp_s(&control_word2, control_word_, _MCW_EM);
    if (err != 0) {
        verbose_abort("Could not restore previous control word");
    }
}
#endif

#ifdef __linux__

#include <VehicleArena/Misc/Floating_Point_Exceptions.hpp>
#include <cfenv>

using namespace VA;

#ifdef __ANDROID__

void VA::enable_floating_point_exceptions()
{}

TemporarilyIgnoreFloatingPointExeptions::TemporarilyIgnoreFloatingPointExeptions() = default;

TemporarilyIgnoreFloatingPointExeptions::~TemporarilyIgnoreFloatingPointExeptions() = default;

#else

void VA::enable_floating_point_exceptions() {
    if (feenableexcept(FE_INVALID) == -1) {
        throw std::runtime_error("Could not enable floating-point exceptions");
    }
}

TemporarilyIgnoreFloatingPointExeptions::TemporarilyIgnoreFloatingPointExeptions()
: fpeflags_{fedisableexcept(FE_ALL_EXCEPT)}
{
    if (fpeflags_ == -1) {
        throw std::runtime_error("Could not disable floating-point exceptions");
    }
}

TemporarilyIgnoreFloatingPointExeptions::~TemporarilyIgnoreFloatingPointExeptions() {
    if (feenableexcept(fpeflags_) == -1) {
        verbose_abort("Could not re-enable floating-point exceptions");
    }
}

#endif
#endif
