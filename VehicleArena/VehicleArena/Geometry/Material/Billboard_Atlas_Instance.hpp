// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Material/Blend_Distances.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstdint>

namespace VA {

enum class ExternalRenderPassType;

struct BillboardAtlasInstance {
    OrderableFixedArray<float, 2> uv_scale = uninitialized;
    OrderableFixedArray<float, 2> uv_offset = uninitialized;
    uint8_t texture_layer;
    OrderableFixedArray<float, 3> vertex_scale = uninitialized;
    ScenePos max_center_distance2;
    ExternalRenderPassType occluder_pass;
    OrderableFixedArray<float, 4> alpha_distances = { default_linear_distances };
    std::partial_ordering operator <=> (const BillboardAtlasInstance&) const = default;
    template <class Archive>
    void serialize(Archive& archive) {
        archive(uv_scale);
        archive(uv_offset);
        archive(texture_layer);
        archive(vertex_scale);
        archive(max_center_distance2);
        archive(occluder_pass);
        archive(alpha_distances);
    }
};

}
