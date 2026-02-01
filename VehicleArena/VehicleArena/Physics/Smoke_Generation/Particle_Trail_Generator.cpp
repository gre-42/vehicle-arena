// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Particle_Trail_Generator.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Particle_Descriptor.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Smoke_Particle_Generator.hpp>

using namespace VA;

ParticleTrailGenerator::ParticleTrailGenerator(SmokeParticleGenerator& smoke_generator)
    : smoke_generator_{ smoke_generator }
    , yangle_rng_{ 121, 0.f, (float)(2. * M_PI) }
{}

ParticleTrailGenerator::~ParticleTrailGenerator() = default;

void ParticleTrailGenerator::generate(
    const FixedArray<ScenePos, 3>& position,
    const FixedArray<float, 3>& rotation,
    const FixedArray<float, 3>& velocity,
    float texture_layer,
    const ParticleDescriptor& trail,
    const std::string& instance_prefix,
    ParticleContainer particle_container,
    const StaticWorld& static_world)
{
    auto r = [&]() {
        switch (trail.rotation) {
        case ParticleRotation::EMITTER:
            return rotation;
        case ParticleRotation::RANDOM_YANGLE:
            return FixedArray<float, 3>{0.f, yangle_rng_(), 0.f};
        }
        throw std::runtime_error("Unknown particle rotation");
    }();
    smoke_generator_.generate_root(
        trail.resource_name,
        VariableAndHash<std::string>{instance_prefix + smoke_generator_.generate_suffix()},
        position,
        r,
        velocity,
        trail.air_resistance_halflife,
        texture_layer,
        trail.animation_duration,
        particle_container,
        static_world);
}
