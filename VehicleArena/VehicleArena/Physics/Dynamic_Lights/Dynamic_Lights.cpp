// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Dynamic_Lights.hpp"
#include <VehicleArena/Physics/Dynamic_Lights/Animated_Dynamic_Light.hpp>
#include <VehicleArena/Physics/Dynamic_Lights/Constant_Dynamic_Light.hpp>
#include <VehicleArena/Physics/Dynamic_Lights/Dynamic_Light_Db.hpp>
#include <VehicleArena/Physics/Units.hpp>

using namespace VA;

DynamicLights::DynamicLights(const DynamicLightDb& db)
    : db_{ db }
{}

DynamicLights::~DynamicLights() = default;

std::unique_ptr<IDynamicLight> DynamicLights::instantiate(
    const std::string& name,
    const std::function<FixedArray<ScenePos, 3>()>& get_position,
    std::chrono::steady_clock::time_point time)
{
    const auto& config = db_.get(name);
    std::unique_ptr<IDynamicLight> result;
    if (std::holds_alternative<AnimatedDynamicLightConfiguration>(config)) {
        result = std::make_unique<AnimatedDynamicLight>(get_position, time, std::get<AnimatedDynamicLightConfiguration>(config), *this);
    } else if (std::holds_alternative<ConstantDynamicLightConfiguration>(config)) {
        result = std::make_unique<ConstantDynamicLight>(get_position, time, std::get<ConstantDynamicLightConfiguration>(config), *this);
    } else {
        throw std::runtime_error("Unknown dynamic light type");
    }
    std::scoped_lock lock{ mutex_ };
    if (!instances_.insert(result.get()).second) {
        verbose_abort("DynamicLights::instantiate internal error");
    }
    return result;
}

void DynamicLights::erase(IDynamicLight& light) {
    std::scoped_lock lock{ mutex_ };
    if (instances_.erase(&light) != 1) {
        verbose_abort("Could not delete dynamic light");
    }
}

void DynamicLights::append_time(std::chrono::steady_clock::time_point time) {
    std::scoped_lock lock{ mutex_ };
    for (const auto& l : instances_) {
        l->append_time(time);
    }
}

void DynamicLights::set_time(std::chrono::steady_clock::time_point time) {
    std::scoped_lock lock{ mutex_ };
    for (auto it = instances_.begin(); it != instances_.end(); ) {
        auto& l = **it;
        if (l.animation_completed(time)) {
            instances_.erase(it++);
        } else {
            l.set_time(time);
            ++it;
        }
    }
}

FixedArray<float, 3> DynamicLights::get_color(const FixedArray<ScenePos, 3>& target_position) const {
    FixedArray<float, 3> result = fixed_zeros<float, 3>();
    std::scoped_lock lock{ mutex_ };
    for (const auto& l : instances_) {
        result += l->get_color(target_position);
    }
    return result;
}
