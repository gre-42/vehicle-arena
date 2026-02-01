// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Animated_Dynamic_Light.hpp"
#include <VehicleArena/Physics/Dynamic_Lights/Dynamic_Lights.hpp>
#include <VehicleArena/Physics/Units.hpp>

using namespace VA;

AnimatedDynamicLight::AnimatedDynamicLight(
    std::function<FixedArray<ScenePos, 3>()> get_position,
    std::chrono::steady_clock::time_point time,
    const AnimatedDynamicLightConfiguration& config,
    DynamicLights& container)
    : creation_time_{ time }
    , get_position_{ std::move(get_position) }
    , position_history_ { get_position_(), time }
    , config_{ config }
    , container_{ container }
    , position_{ fixed_nans<ScenePos, 3>() }
    , color_{ fixed_nans<float, 3>() }
{}

AnimatedDynamicLight::~AnimatedDynamicLight() {
    container_.erase(*this);
}

void AnimatedDynamicLight::append_time(std::chrono::steady_clock::time_point time)
{
    position_history_.append(get_position_(), time);
}

void AnimatedDynamicLight::set_time(std::chrono::steady_clock::time_point time) {
    position_ = position_history_.get(time);
    color_ = config_.time_to_color(elapsed(time));
}

FixedArray<float, 3> AnimatedDynamicLight::get_color(const FixedArray<ScenePos, 3>& target_position) const
{
    auto dist2 = sum(squared(position_ - target_position));
    return config_.squared_distance_to_intensity(dist2) * color_;
}

bool AnimatedDynamicLight::animation_completed(std::chrono::steady_clock::time_point time) const
{
    return elapsed(time) > config_.time_to_color.xmax();
}

float AnimatedDynamicLight::elapsed(std::chrono::steady_clock::time_point time) const {
    return std::chrono::duration<float>(time - creation_time_).count() * seconds;
}
