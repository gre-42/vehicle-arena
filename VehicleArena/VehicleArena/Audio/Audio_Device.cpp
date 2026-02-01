// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Device.hpp"
#include <VehicleArena/Audio/OpenALSoft_efx.h>
#include <VehicleArena/Memory/Integral_Cast.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <stdexcept>
#include <stdexcept>

using namespace VA;

AudioDevice::AudioDevice() {
    // select the "preferred device"
    device_ = alcOpenDevice(nullptr);
    if (device_ == nullptr) {
        throw std::runtime_error("Could not open audio device");
    }
}

AudioDevice::~AudioDevice() {
    if (!alcCloseDevice(device_)) {
        verbose_abort("Could not close audio device");
    }
}

unsigned int AudioDevice::get_frequency() const {
    ALCint rate;
    alcGetIntegerv(device_, ALC_FREQUENCY, 1, &rate);
    ALCenum error = alcGetError(device_);
    if (error != ALC_NO_ERROR) {
        throw std::runtime_error("Could not read audio frequency, code: " + std::to_string(error));
    }
    return integral_cast<unsigned int>(rate);
}

std::string AudioDevice::get_name() const {
    const auto* name = alcGetString(device_, ALC_DEVICE_SPECIFIER);
    ALCenum error = alcGetError(device_);
    if (error != ALC_NO_ERROR) {
        throw std::runtime_error("Could not read audio device name, code: " + std::to_string(error));
    }
    if (name == nullptr) {
        throw std::runtime_error("Could not read audio device name");
    }
    return name;
}

unsigned int AudioDevice::get_max_auxiliary_sends() const {
    ALCint max_auxiliary_sends = 0;
    alcGetIntegerv(device_, ALC_MAX_AUXILIARY_SENDS, 1, &max_auxiliary_sends);
    if (ALCenum error = alcGetError(device_); error != ALC_NO_ERROR) {
        throw std::runtime_error("Could obtain max auxiliary sends per source, code: " + std::to_string(error));
    }
    return integral_cast<unsigned int>(max_auxiliary_sends);
}
