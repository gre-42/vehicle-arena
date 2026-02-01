// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Material/Shading.hpp>
#include <cstdint>

namespace VA {

struct ShadingFactors;
enum class PhysicsMaterial: uint32_t;

// See the "plot_fresnel.py" script for tuning these parameters.

Shading material_shading(PhysicsMaterial material, const ShadingFactors& factors);
Shading material_shading(const Shading& shading, const ShadingFactors& factors);

namespace RawShading {
    using O = OrderableFixedArray<float, 3>;
    static const Shading CURB{ .specular = O(0.5f), .fresnel = {.reflectance = {.min = 0.f, .max = 0.15f, .exponent = 5.f}, .ambient = O(1.f)} };
    static const Shading MUD{ .specular = O(0.5f), .fresnel = {.reflectance = {.min = 0.f, .max = 0.15f, .exponent = 5.f}, .ambient = O(1.f)} };

    static const Shading ROOF{ .specular = O(0.2f), .fresnel = {.reflectance = {.min = 0.f, .max = 0.7f, .exponent = 5.f}, .ambient = O(1.f)} };
    static const Shading CEILING{ .specular = O(0.2f), .fresnel = {.reflectance = {.min = 0.f, .max = 0.15f, .exponent = 5.f}, .ambient = O(1.f)} };
    static const Shading DEFAULT{ .fresnel = {.reflectance = {.min = 0.f, .max = 0.15f, .exponent = 5.f}, .ambient = O(1.f)} };
}

}
