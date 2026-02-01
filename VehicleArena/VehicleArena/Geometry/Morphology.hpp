// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Material/Blend_Distances.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <compare>

namespace VA {

enum class PhysicsMaterial: uint32_t;

struct Morphology {
    PhysicsMaterial physics_material;
    SquaredStepDistances center_distances2 = default_step_distances2;
    float max_triangle_distance = INFINITY;
    float object_cluster_width = 0.f;
    float triangle_cluster_width = 0.f;
    std::partial_ordering operator <=> (const Morphology&) const = default;
    template <class Archive>
    void serialize(Archive& archive) {
        archive(physics_material);
        archive(center_distances2);
        archive(max_triangle_distance);
        archive(object_cluster_width);
        archive(triangle_cluster_width);
    }
};

Morphology& operator += (Morphology& a, PhysicsMaterial b);
Morphology& operator -= (Morphology& a, PhysicsMaterial b);
Morphology operator + (const Morphology& a, PhysicsMaterial b);
Morphology operator - (const Morphology& a, PhysicsMaterial b);

}
