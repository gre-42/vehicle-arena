// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Constant_Particle_Trail.hpp>
#include <cstdint>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

namespace VA {

enum class PhysicsMaterial: uint32_t;
enum class RigidBodyVehicleFlags;
enum class ParticleContainer;

struct BulletExplosion {
    std::optional<std::unordered_set<PhysicsMaterial>> materials;
    VariableAndHash<std::string> resource_name;
    ParticleContainer particle_container;
    float animation_time;
    VariableAndHash<std::string> audio_resource_name;
};

struct BulletProperties {
    VariableAndHash<std::string> renderable_resource_name;
    VariableAndHash<std::string> hitbox_resource_name;
    VariableAndHash<std::string> engine_audio_resource_name;
    std::vector<BulletExplosion> explosions;
    RigidBodyVehicleFlags rigid_body_flags;
    float mass;
    float velocity;
    float max_lifetime;
    float damage;
    float damage_radius;
    FixedArray<float, 3> size = uninitialized;
    ConstantParticleTrail trail;
    VariableAndHash<std::string> trace_storage;
    std::string dynamic_light_configuration_before_impact;
    std::string dynamic_light_configuration_after_impact;
};

}
