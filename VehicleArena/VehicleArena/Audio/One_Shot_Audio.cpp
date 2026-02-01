// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "One_Shot_Audio.hpp"
#include <VehicleArena/Audio/Audio_Periodicity.hpp>
#include <VehicleArena/Audio/Audio_Scene.hpp>
#include <VehicleArena/Geometry/Primitives/Interval.hpp>
#include <VehicleArena/Memory/Event_Emitter.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <mutex>

using namespace VA;

AudioSourceAndPosition::AudioSourceAndPosition(
    const AudioBuffer& buffer,
    PositionRequirement position_requirement,
    float alpha,
    const AudioSourceState<ScenePos>& position)
    : source{ buffer, position_requirement, alpha }
    , position{ position }
{}

AudioSourceAndPosition::~AudioSourceAndPosition() = default;

OneShotAudio::OneShotAudio(
    PositionRequirement position_requirement,
    std::function<bool()> paused,
    EventEmitter<>& paused_changed)
    : position_requirement_{ position_requirement }
    , paused_{ std::move(paused) }
    , erdt_{ paused_changed, [this](){ advance_time(); } }
{}

OneShotAudio::~OneShotAudio() {
    erdt_.reset();
    on_destroy.clear();
}

void OneShotAudio::advance_time(float dt, const StaticWorld& world) {
    advance_time();
}

void OneShotAudio::advance_time() {
    bool pause = paused_();
    std::scoped_lock lock{ mutex_ };
    sources_.remove_if([](auto& sp){
        return sp->source.stopped();
    });
    if (pause) {
        for (auto &sp : sources_) {
            sp->source.pause();
        }
    } else {
        for (auto &sp : sources_) {
            sp->source.unpause();
        }
    }
    for (auto& sp : sources_) {
        AudioScene::set_source_transformation(sp->source, sp->position);
    }
    // if (!pause && !sources_.empty()) {
    //     lraw() << "OneShotAudio::advance_time";
    //     for (auto& sp : sources_) {
    //         lraw() << &sp.source;
    //     }
    //     AudioScene::print(lraw().ref());
    // }
}

std::shared_ptr<AudioSourceAndPosition> OneShotAudio::play(
    const AudioBuffer& audio_buffer,
    const AudioLowpass* lowpass,
    const AudioSourceState<ScenePos>& position,
    AudioPeriodicity periodicity,
    const std::optional<Interval<float>>& distance_clamping,
    float gain,
    float alpha)
{
    std::scoped_lock lock{ mutex_ };
    auto sp = sources_.emplace_back(std::make_shared<AudioSourceAndPosition>(
        audio_buffer,
        position_requirement_,
        alpha,
        position));
    AudioScene::set_source_transformation(sp->source, sp->position);
    sp->source.set_loop(periodicity == AudioPeriodicity::PERIODIC);
    sp->source.set_gain(gain);
    if (distance_clamping.has_value()) {
        sp->source.set_distance_clamping(*distance_clamping);
    }
    if (lowpass != nullptr) {
        sp->source.set_lowpass(*lowpass);
    }
    sp->source.play();
    return sp;
}

void OneShotAudio::stop() {
    std::scoped_lock lock{ mutex_ };
    sources_.clear();
}
