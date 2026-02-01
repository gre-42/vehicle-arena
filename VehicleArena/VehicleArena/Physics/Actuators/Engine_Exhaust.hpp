// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Audio/Audio_Entity_State.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Actuators/IEngine_Event_Listener.hpp>
#include <VehicleArena/Physics/Maybe_Generate.hpp>
#include <VehicleArena/Physics/Rotating_Frame.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Constant_Particle_Trail.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Particle_Trail_Generator.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Smoke_Particle_Generator.hpp>
#include <optional>

namespace VA {

class RenderingResources;
class SceneNodeResources;
class IParticleRenderer;
class Scene;
class RigidBodies;

class EngineExhaust: public IEngineEventListener {
public:
    explicit EngineExhaust(
        RenderingResources& rendering_resources,
        SceneNodeResources& scene_node_resources,
        std::shared_ptr<IParticleRenderer> particle_renderer,
        Scene& scene,
        RigidBodies& rigid_bodies,
        const ConstantParticleTrail& particle,
        const TransformationMatrix<SceneDir, ScenePos, 3>& relative_location,
        float p_reference);
    virtual ~EngineExhaust() override;
    virtual void notify_rotation(
        float engine_angular_velocity,
        float tires_angular_velocity,
        const EnginePowerIntent& engine_power_intent,
        float max_surface_power,
        const StaticWorld& static_world) override;
    virtual void set_location(
        const RotatingFrame<SceneDir, ScenePos, 3>& frame) override;
    virtual void advance_time(float dt) override;
private:
    MaybeGenerate maybe_generate_;
    SmokeParticleGenerator smoke_generator_;
    ParticleTrailGenerator trail_generator_;
    ConstantParticleTrail particle_;
    TransformationMatrix<SceneDir, ScenePos, 3> relative_location_;
    float p_reference_;
};

}
