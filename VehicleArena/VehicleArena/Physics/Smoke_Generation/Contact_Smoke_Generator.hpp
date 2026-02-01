// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Destruction_Observer.hpp>
#include <VehicleArena/Physics/Maybe_Generate.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Particle_Trail_Generator.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <compare>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>

namespace VA {

class SmokeParticleGenerator;
class RigidBodyVehicle;
struct IntersectionScene;
struct SurfaceSmokeInfo;
class OneShotAudio;
struct PhysicsEngineConfig;
struct PhysicsPhase;

struct ContactEmissions {
    MaybeGenerate maybe_generate;
    std::optional<ParticleTrailGenerator> particle_trail_generator;
    std::optional<FixedArray<ScenePos, 3>> old_position;
};

struct ContactSmokeAndAudio {
    std::map<std::pair<size_t, const SurfaceSmokeInfo*>, ContactEmissions> smoke;
    std::map<size_t, ContactEmissions> audio;
};

class ContactSmokeGenerator: public DestructionObserver<const RigidBodyVehicle&>, public virtual DanglingBaseClass {
public:
    ContactSmokeGenerator(
        OneShotAudio& one_shot_audio,
        SmokeParticleGenerator& air_smoke_particle_generator,
        SmokeParticleGenerator& skidmark_smoke_particle_generator,
        SmokeParticleGenerator& sea_wave_smoke_particle_generator);
    ~ContactSmokeGenerator();
    virtual void notify_destroyed(const RigidBodyVehicle& destroyed_object) override;

    void notify_contact(
        const FixedArray<ScenePos, 3>& intersection_point,
        const FixedArray<float, 3>& rotation,
        const FixedArray<SceneDir, 3>& surface_normal,
        const IntersectionScene& c);
    void advance_time(
        RigidBodyVehicle& vehicle,
        const PhysicsEngineConfig& cfg,
        const PhysicsPhase& phase);
private:
    OneShotAudio& one_shot_audio_;
    SmokeParticleGenerator& air_smoke_particle_generator_;
    SmokeParticleGenerator& skidmark_smoke_particle_generator_;
    SmokeParticleGenerator& sea_wave_smoke_particle_generator_;
    std::unordered_map<RigidBodyVehicle*, ContactSmokeAndAudio> tire_smoke_trail_generators_;
};

}
