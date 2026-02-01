// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Engine_Event_Listeners.hpp"

using namespace VA;

EngineEventListeners::EngineEventListeners() = default;

EngineEventListeners::~EngineEventListeners() = default;

void EngineEventListeners::notify_rotation(
    float engine_angular_velocity,
    float tires_angular_velocity,
    const EnginePowerIntent& engine_power_intent,
    float max_surface_power,
    const StaticWorld& static_world)
{
    for (const auto& l : listeners_) {
        l->notify_rotation(
            engine_angular_velocity,
            tires_angular_velocity,
            engine_power_intent,
            max_surface_power,
            static_world);
    }
}

void EngineEventListeners::set_location(const RotatingFrame<SceneDir, ScenePos, 3>& frame)
{
    for (const auto& l : listeners_) {
        l->set_location(frame);
    }
}

void EngineEventListeners::advance_time(float dt) {
    for (const auto& l : listeners_) {
        l->advance_time(dt);
    }
}

void EngineEventListeners::add(std::shared_ptr<IEngineEventListener> l) {
    listeners_.emplace_back(std::move(l));
}
