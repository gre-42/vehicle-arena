// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#ifdef _MSC_VER

#include "Sleep.hpp"
#include <VehicleArena/Memory/Integral_Cast.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <Windows.h>

using namespace VA;

// From: https://stackoverflow.com/questions/5801813/c-usleep-is-obsolete-workarounds-for-windows-mingw
void VA::usleep(uint64_t usec) 
{
    LARGE_INTEGER ft{
        .QuadPart = -(10 * integral_cast<int64_t>(usec)) // Convert to 100 nanosecond interval, negative value indicates relative time
    };

    HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
    if (time == NULL) {
        verbose_abort("CreateWaitableTimer failed");
    }
    if (!SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0)) {
        verbose_abort("SetWaitableTimer failed");
    }
    if (WaitForSingleObject(timer, INFINITE) == WAIT_FAILED) {
        verbose_abort("WaitForSingleObject failed");
    }
    if (!CloseHandle(timer)) {
        verbose_abort("CloseHandle failed");
    }
}

#endif
