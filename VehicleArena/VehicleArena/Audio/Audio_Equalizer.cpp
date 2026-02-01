// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Equalizer.hpp"
#include <VehicleArena/Audio/Audio_Resources.hpp>
#include <VehicleArena/Audio/CHK.hpp>
#include <VehicleArena/Audio/OpenALSoft_efx.h>

using namespace VA;

AudioEqualizer::AudioEqualizer()
{
    AL_CHK(alGenEffects(1, &handle_));
}

AudioEqualizer::~AudioEqualizer() {
    AL_ABORT(alDeleteEffects((ALuint)1, &handle_));
}

std::shared_ptr<AudioEqualizer> AudioEqualizer::create(const AudioEqualizerInformation& parameters) {
    auto res = std::make_shared<AudioEqualizer>();
    AL_CHK(alEffecti(res->handle_, AL_EFFECT_TYPE, AL_EFFECT_EQUALIZER));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_LOW_GAIN, parameters.low_gain));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_LOW_CUTOFF, parameters.low_cutoff));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_MID1_GAIN, parameters.mid1_gain));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_MID1_CENTER, parameters.mid1_center));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_MID1_WIDTH, parameters.mid1_width));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_MID2_GAIN, parameters.mid2_gain));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_MID2_CENTER, parameters.mid2_center));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_MID2_WIDTH, parameters.mid2_width));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_HIGH_GAIN, parameters.high_gain));
    AL_CHK(alEffectf(res->handle_, AL_EQUALIZER_HIGH_CUTOFF, parameters.high_cutoff));
    return res;
}
