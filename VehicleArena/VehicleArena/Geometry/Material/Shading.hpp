// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Material/Blend_Distances.hpp>
#include <VehicleArena/Geometry/Material/Fresnel.hpp>
#include <compare>
#include <nlohmann/json_fwd.hpp>

namespace VA {

struct Shading {
    OrderableFixedArray<float, 3> emissive{ 0.f, 0.f, 0.f };
    OrderableFixedArray<float, 3> ambient{ 1.f, 1.f, 1.f };
    OrderableFixedArray<float, 3> diffuse{ 0.8f, 0.8f, 0.8f };
    OrderableFixedArray<float, 3> specular{ 0.5f, 0.5f, 0.5f };
    float specular_exponent = 4.f;
    OrderableFixedArray<float, 3> reflectance{ 0.f, 0.f, 0.f };
    FresnelAndAmbient fresnel;
    OrderableFixedArray<float, 2> fog_distances{ default_step_distances };
    OrderableFixedArray<float, 3> fog_ambient{ 1.f, 1.f, 1.f };
    template <class Archive>
    void serialize(Archive& archive) {
        archive(emissive);
        archive(ambient);
        archive(diffuse);
        archive(specular);
        archive(specular_exponent);
        archive(reflectance);
        archive(fresnel);
        archive(fog_distances);
        archive(fog_ambient);
    }
    std::partial_ordering operator <=> (const Shading&) const = default;
};

void from_json(const nlohmann::json& j, Shading& shading);

}
