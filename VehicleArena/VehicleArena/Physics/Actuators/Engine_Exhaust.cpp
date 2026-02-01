// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Engine_Exhaust.hpp"
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>

using namespace VA;

EngineExhaust::EngineExhaust(
    RenderingResources& rendering_resources,
    SceneNodeResources& scene_node_resources,
    std::shared_ptr<IParticleRenderer> particle_renderer,
    Scene& scene,
    RigidBodies& rigid_bodies,
    const ConstantParticleTrail& particle,
    const TransformationMatrix<SceneDir, ScenePos, 3>& relative_location,
    float p_reference)
    : smoke_generator_{
        rendering_resources,
        scene_node_resources,
        particle_renderer,
        scene,
        rigid_bodies }
    , trail_generator_{ smoke_generator_ }
    , particle_{ particle }
    , relative_location_{ relative_location }
    , p_reference_{ p_reference }
{}

EngineExhaust::~EngineExhaust() = default;

void EngineExhaust::notify_rotation(
    float engine_angular_velocity,
    float tires_angular_velocity,
    const EnginePowerIntent& engine_power_intent,
    float max_surface_power,
    const StaticWorld& static_world)
{
    float p = engine_power_intent.real_power(
        engine_angular_velocity,
        tires_angular_velocity,
        max_surface_power);
    if ((p >= p_reference_) && maybe_generate_(particle_.generation_dt)) {
        trail_generator_.generate(
            relative_location_.t,
            matrix_2_tait_bryan_angles(relative_location_.R),
            fixed_zeros<SceneDir, 3>(),     // velocity
            0.f,                            // texture_layer
            particle_.particle,
            "exhaust",
            ParticleContainer::INSTANCE,
            static_world);
    }
}

void EngineExhaust::set_location(
    const RotatingFrame<SceneDir, ScenePos, 3>& frame)
{}

void EngineExhaust::advance_time(float dt) {
    maybe_generate_.advance_time(dt);
}
