// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Physics/Containers/Rigid_Bodies.hpp>
#include <VehicleArena/Physics/Interfaces/Collision_Observer.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Physics/Maybe_Generate.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Particle_Trail_Generator.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <functional>
#include <mutex>
#include <string>

namespace VA {

class RigidBodies;
class RigidBodyVehicle;
class RigidBodyPulses;
class Scene;
class SceneNode;
class AdvanceTimes;
class SceneNodeResources;
class IPlayer;
class ITeam;
class SmokeParticleGenerator;
class ITrailExtender;
class IDynamicLight;
class DynamicLights;
struct BulletProperties;
template <class TPosition>
struct AudioSourceState;

enum class RotateBullet {
    YES,
    NO
};

class Bullet final:
    public CollisionObserver,
    public IAdvanceTime,
    public virtual DanglingBaseClass
{
public:
    Bullet(
        Scene& scene,
        std::function<void(const AudioSourceState<ScenePos>&, const VariableAndHash<std::string>&)> generate_bullet_explosion_audio,
        std::function<void(const AudioSourceState<ScenePos>*)> update_engine_audio_position,
        SmokeParticleGenerator& smoke_generator,
        AdvanceTimes& advance_times,
        RigidBodyVehicle& rigid_body,
        RigidBodies& rigid_bodies,
        IPlayer* gunner,
        ITeam* team,
        VariableAndHash<std::string> bullet_node_name,
        const BulletProperties& props,
        std::unique_ptr<ITrailExtender> trace_extender,
        DynamicLights& dynamic_lights,
        const StaticWorld& world,
        RotateBullet rotate_bullet);
    ~Bullet();
    virtual void advance_time(float dt, const StaticWorld& world) override;
    virtual void notify_collided(
        const FixedArray<ScenePos, 3>& intersection_point,
        const StaticWorld& world,
        RigidBodyVehicle& rigid_body,
        PhysicsMaterial physics_material,
        CollisionRole collision_role,
        CollisionType& collision_type,
        bool& abort) override;
private:
    void cause_damage(
        const FixedArray<ScenePos, 3>& intersection_point,
        RigidBodyVehicle& rigid_body);
    void cause_damage(
        RigidBodyVehicle& rigid_body,
        float amount);
    void notify_kill(RigidBodyVehicle& rigid_body_vehicle);
    Scene& scene_;
    std::function<void(const AudioSourceState<ScenePos>&, const VariableAndHash<std::string>&)> generate_bullet_explosion_audio_;
    std::function<void(const AudioSourceState<ScenePos>*)> update_engine_audio_position_;
    SmokeParticleGenerator& smoke_generator_;
    AdvanceTimes& advance_times_;
    RigidBodyPulses& rigid_body_pulses_;
    RigidBodies& rigid_bodies_;
    IPlayer* gunner_;
    ITeam* team_;
    VariableAndHash<std::string> bullet_node_name_;
    const BulletProperties& props_;
    float lifetime_;
    MaybeGenerate maybe_generate_;
    ParticleTrailGenerator trail_generator_;
    bool has_trail_;
    std::unique_ptr<ITrailExtender> trace_extender_;
    std::unique_ptr<IDynamicLight> light_before_impact_;
    std::unique_ptr<IDynamicLight> light_after_impact_;
    bool rotate_bullet_;
    DynamicLights& dynamic_lights_;
    DestructionFunctionsRemovalTokens gunner_on_destroy_;
    DestructionFunctionsRemovalTokens team_on_destroy_;
};

}
