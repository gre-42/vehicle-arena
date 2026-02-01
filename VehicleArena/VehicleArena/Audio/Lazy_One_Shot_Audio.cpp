// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Lazy_One_Shot_Audio.hpp"
#include <VehicleArena/Audio/Audio_Resources.hpp>
#include <VehicleArena/Audio/One_Shot_Audio.hpp>

using namespace VA;

LazyOneShotAudio::LazyOneShotAudio(
    AudioResources& resources,
    VariableAndHash<std::string> resource_name,
    float alpha)
    : info_{ nullptr }
    , alpha_{ alpha }
    , resources_{ resources }
    , resource_name_{ std::move(resource_name) }
{}

void LazyOneShotAudio::preload() {
    if (buffer_ == nullptr) {
        buffer_ = resources_.get_buffer(resource_name_);
        info_ = &resources_.get_buffer_meta(resource_name_);
    }
}

std::shared_ptr<AudioSourceAndPosition> LazyOneShotAudio::play(
    OneShotAudio& one_shot_audio,
    AudioPeriodicity periodicity,
    const AudioSourceState<ScenePos>& position)
{
    preload();
    return one_shot_audio.play(
        *buffer_,
        info_->lowpass.get(),
        position,
        periodicity,
        info_->distance_clamping,
        info_->gain,
        alpha_);
}
