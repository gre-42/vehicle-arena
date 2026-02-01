// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Material_Colors.hpp"
#include <VehicleArena/Geometry/Material_Configuration/Shading_Factors.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <map>

using namespace VA;

Shading VA::material_shading(PhysicsMaterial material, const ShadingFactors& factors) {
    using O = OrderableFixedArray<float, 3>;
    const auto f0 = FresnelAndAmbient{.reflectance = {.min = 0.f, .max = 0.3f, .exponent = 4.f}, .ambient = O(1.f)};
    const auto f1 = FresnelAndAmbient{.reflectance = {.min = 0.f, .max = 0.3f, .exponent = 5.f}, .ambient = O(1.f)};
    const auto f2 = FresnelAndAmbient{.reflectance = {.min = 0.f, .max = 0.15f, .exponent = 5.f}, .ambient = O(1.f)};
    const auto f3 = FresnelAndAmbient{.reflectance = {.min = 0.f, .max = 0.1f, .exponent = 10.f}, .ambient = O(1.f)};
    const auto f4 = FresnelAndAmbient{.reflectance = {.min = 0.f, .max = 0.5f, .exponent = 5.f}, .ambient = O(1.f)};
    const auto f5 = FresnelAndAmbient{.reflectance = {.min = 0.7f, .max = 1.f, .exponent = 5.f}, .ambient = O(0.f)};
    static const std::map<PhysicsMaterial, Shading> m{
        {PhysicsMaterial::NONE, Shading{ .emissive = O{1.f, 0.f, 1.f}, .ambient = O(0.f), .diffuse = O(0.f), .specular = O(0.f) }},
        {PhysicsMaterial::SURFACE_BASE_TARMAC, Shading{ .specular = O(0.5f), .fresnel = f0 }},
        {PhysicsMaterial::SURFACE_BASE_GRAVEL, Shading{ .specular = O(0.f), .fresnel = f1 }},
        {PhysicsMaterial::SURFACE_BASE_SNOW, Shading{ .specular = O(0.f), .fresnel = f1 }},
        {PhysicsMaterial::SURFACE_BASE_ICE, Shading{ .specular = O(0.f), .fresnel = f1 }},
        {PhysicsMaterial::SURFACE_BASE_SAND, Shading{ .specular = O(0.f), .fresnel = f2 }},
        {PhysicsMaterial::SURFACE_BASE_GRASS, Shading{ .specular = O(0.f), .fresnel = f3 }},
        {PhysicsMaterial::SURFACE_BASE_DIRT, Shading{ .specular = O(0.f), .fresnel = f1 }},
        {PhysicsMaterial::SURFACE_BASE_STONE, Shading{ .specular = O(0.f), .fresnel = f2 }},
        {PhysicsMaterial::SURFACE_BASE_FOLIAGE, Shading{ .specular = O(0.f), .fresnel = f4 }},
        {PhysicsMaterial::SURFACE_BASE_GLASS, Shading{ .specular = O(0.f), .reflectance = O(1.f), .fresnel = f5 }},
        {PhysicsMaterial::SURFACE_BASE_WATER, Shading{ .diffuse = O(1.f), .specular = O(0.5f), .fresnel = f0 }},
        {PhysicsMaterial::SURFACE_BASE_TARMAC | PhysicsMaterial::SURFACE_WET, Shading{ .specular = O(0.f), .reflectance = O(1.f), .fresnel = f5 }},
        {PhysicsMaterial::SURFACE_BASE_DIRT | PhysicsMaterial::SURFACE_WET, Shading{ .specular = O(0.f), .reflectance = O(1.f), .fresnel = f5 }},
        {PhysicsMaterial::SURFACE_BASE_DESERT_ROAD, Shading{ .specular = O(0.f), .fresnel = f2 }},
    };
    auto it = m.find(material);
    if (it == m.end()) {
        throw std::runtime_error("Cannot find shading options for material \"" + physics_material_to_string(material));
    }
    return material_shading(it->second, factors);
}

Shading VA::material_shading(const Shading& shading, const ShadingFactors& factors) {
    Shading res = shading;
    res.emissive *= factors.emissive_factor;
    res.ambient *= factors.ambient_factor;
    res.diffuse *= factors.diffuse_factor;
    res.specular *= factors.specular_factor;
    res.fresnel.ambient *= factors.fresnel_ambient_factor;
    res.fog_distances = factors.fog_distances;
    res.fog_ambient = factors.fog_ambient;
    return res;
}
