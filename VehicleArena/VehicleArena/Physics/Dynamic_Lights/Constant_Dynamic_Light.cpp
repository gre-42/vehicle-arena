// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Constant_Dynamic_Light.hpp"
#include <VehicleArena/Physics/Dynamic_Lights/Dynamic_Lights.hpp>

using namespace VA;

ConstantDynamicLight::ConstantDynamicLight(
    std::function<FixedArray<ScenePos, 3>()> get_position,
    std::chrono::steady_clock::time_point time,
    const ConstantDynamicLightConfiguration& config,
    DynamicLights& container)
    : get_position_{ std::move(get_position) }
    , position_{ get_position_() }
    , position_history_ { position_, time }
    , config_{ config }
    , container_{ container }
{}

ConstantDynamicLight::~ConstantDynamicLight() {
    container_.erase(*this);
}

void ConstantDynamicLight::append_time(std::chrono::steady_clock::time_point time)
{
    position_history_.append(get_position_(), time);
}

void ConstantDynamicLight::set_time(std::chrono::steady_clock::time_point time)
{
    position_ = position_history_.get(time);
}

FixedArray<float, 3> ConstantDynamicLight::get_color(const FixedArray<ScenePos, 3>& target_position) const
{
    auto dist2 = sum(squared(position_ - target_position));
    return config_.squared_distance_to_intensity(dist2) * config_.color;
}

bool ConstantDynamicLight::animation_completed(std::chrono::steady_clock::time_point time) const
{
    return false;
}
