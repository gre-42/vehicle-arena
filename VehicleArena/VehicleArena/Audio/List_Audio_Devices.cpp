// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "List_Audio_Devices.hpp"
#include <VehicleArena/Audio/CHK.hpp>
#include <VehicleArena/Audio/OpenAL_al.h>
#include <VehicleArena/Audio/OpenAL_alc.h>
#include <cstddef>
#include <cstring>
#include <mutex>
#include <stdexcept>

using namespace VA;

void VA::list_audio_devices(std::ostream& ostr) {
    std::scoped_lock lock{ al_error_mutex };
    const ALCchar* devices = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    if (devices == nullptr) {
        throw std::runtime_error("Could not list audio devices");
    }
    const ALCchar* device = devices;
    const ALCchar* next = devices + 1;
    size_t len = 0;

    ostr << "Audio devices:\n";
    while (device && *device != '\0' && next && *next != '\0') {
        ostr << "  - " << device << '\n';
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }
}
