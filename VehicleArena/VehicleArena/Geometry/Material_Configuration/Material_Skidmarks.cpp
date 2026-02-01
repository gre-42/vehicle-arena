// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Material_Skidmarks.hpp"
#include <VehicleArena/Geometry/Material/Particle_Type.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <map>
#include <stdexcept>

using namespace VA;

ParticleType VA::material_skidmarks(PhysicsMaterial material) {
    static const std::map<PhysicsMaterial, ParticleType> m{
        {PhysicsMaterial::NONE, ParticleType::NONE},
        {PhysicsMaterial::SURFACE_BASE_TARMAC, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_GRAVEL, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_SNOW, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_ICE, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_SAND, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_GRASS, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_DIRT, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_STONE, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_FOLIAGE, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_GLASS, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_WATER, ParticleType::WATER_WAVE | ParticleType::SEA_SPRAY},
        {PhysicsMaterial::SURFACE_BASE_TARMAC | PhysicsMaterial::SURFACE_WET, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_DIRT | PhysicsMaterial::SURFACE_WET, ParticleType::SKIDMARK},
        {PhysicsMaterial::SURFACE_BASE_DESERT_ROAD, ParticleType::SKIDMARK},
    };
    auto it = m.find(material);
    if (it == m.end()) {
        throw std::runtime_error("Cannot find skidmark options for material \"" + physics_material_to_string(material));
    }
    return it->second;
}
