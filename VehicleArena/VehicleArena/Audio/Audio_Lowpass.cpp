// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Lowpass.hpp"
#include <VehicleArena/Audio/Audio_Resources.hpp>
#include <VehicleArena/Audio/CHK.hpp>
#include <VehicleArena/Audio/OpenALSoft_efx.h>

using namespace VA;

AudioLowpass::AudioLowpass()
{
    AL_CHK(alGenFilters(1, &handle_));
}

AudioLowpass::~AudioLowpass() {
    AL_ABORT(alDeleteFilters((ALuint)1, &handle_));
}

std::shared_ptr<AudioLowpass> AudioLowpass::create(const AudioLowpassInformation& parameters) {
    auto res = std::make_shared<AudioLowpass>();
    AL_CHK(alFilteri(res->handle_, AL_FILTER_TYPE, AL_FILTER_LOWPASS));
    AL_CHK(alFilterf(res->handle_, AL_LOWPASS_GAIN, parameters.gain));
    AL_CHK(alFilterf(res->handle_, AL_LOWPASS_GAINHF, parameters.gain_hf));
    return res;
}
