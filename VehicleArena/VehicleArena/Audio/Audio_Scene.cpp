// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Scene.hpp"
#include <VehicleArena/Audio/Audio_Distance_Model.hpp>
#include <VehicleArena/Audio/Audio_Entity_State.hpp>
#include <VehicleArena/Audio/Audio_Listener.hpp>
#include <VehicleArena/Audio/Audio_Source.hpp>
#include <VehicleArena/Audio/CHK.hpp>
#include <mutex>
#include <string>

using namespace VA;

FastMutex AudioScene::mutex_;
float AudioScene::default_alpha_ = 1.f;
VerboseUnorderedMap<const AudioSource*, AudioSourceNode> AudioScene::source_nodes_{
    "Audio source",
    [](const AudioSource* s) { return (std::stringstream() << s).str(); }
};

void AudioScene::set_default_alpha(float alpha) {
    std::scoped_lock lock{ mutex_ };
    default_alpha_ = alpha;
}

void AudioScene::add_source(AudioSource& source, float alpha) {
    std::scoped_lock lock{ mutex_ };
    source_nodes_.add(&source, AudioSourceNode{
        .relative_position{std::isnan(alpha) ? default_alpha_ : alpha, uninitialized},
        .relative_velocity{std::isnan(alpha) ? default_alpha_ : alpha, uninitialized}
        });
}

void AudioScene::remove_source(AudioSource& source) {
    std::scoped_lock lock{ mutex_ };
    source_nodes_.remove(&source);
}

void AudioScene::set_listener(
    const TransformationMatrix<float, ScenePos, 3>& pose,
    const FixedArray<float, 3>& velocity)
{
    std::scoped_lock lock{ mutex_ };
    AudioListener::set_transformation(AudioListenerState{
        .pose = pose,
        .velocity = velocity
        });
}

void AudioScene::set_source_transformation(
    AudioSource& source,
    const AudioSourceState<ScenePos>& state)
{
    auto relpos = AudioListener::get_relative_position(state);
    if (!relpos.has_value()) {
        return;
    }
    std::scoped_lock lock{ mutex_ };
    auto& node = source_nodes_.get(&source);
    const auto& smooth_position = node.relative_position(relpos->position);
    const auto& smooth_velocity = node.relative_velocity(relpos->velocity);
    source.set_position(AudioSourceState<float>{
        .position = smooth_position,
        .velocity = smooth_velocity
    });
}

void AudioScene::set_distance_model(AudioDistanceModel model) {
    switch (model) {
    case AudioDistanceModel::INVERSE_DISTANCE_CLAMPED:
        AL_CHK(alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED));
        return;
    case AudioDistanceModel::LINEAR_DISTANCE_CLAMPED:
        AL_CHK(alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED));
        return;
    }
    throw std::runtime_error("Unknown audio distance model: " + std::to_string((int)model));
}

void AudioScene::print(std::ostream& ostr) {
    std::scoped_lock lock{ mutex_ };
    for (const auto& [source, node] : source_nodes_) {
        ostr << source << " pos: ";
        {
            const auto& rpos = node.relative_position.xhat();
            if (rpos.has_value()) {
                ostr << *rpos;
            } else {
                ostr << "?";
            }
        }
        ostr << " | vel: ";
        {
            const auto& rvel = node.relative_velocity.xhat();
            if (rvel.has_value()) {
                ostr << *rvel;
            } else {
                ostr << "?";
            }
        }
        ostr << '\n';
    }
}
