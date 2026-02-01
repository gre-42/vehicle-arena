// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Cluster_Meshes.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Material.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Modifiers/Group_Meshes_By_Material.hpp>
#include <VehicleArena/Geometry/Mesh/Modifiers/Material_And_Morphology.hpp>
#include <VehicleArena/Geometry/Mesh/Modifiers/Merge_Meshes.hpp>
#include <VehicleArena/Geometry/Mesh/Modifiers/Position_And_Meshes.hpp>
#include <VehicleArena/Strings/Group_And_Name.hpp>
#include <unordered_map>

using namespace VA;

template <class TPos>
std::list<PositionAndMeshes<TPos>> VA::cluster_meshes(
    const std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas,
    const std::function<FixedArray<TPos, 3>(const ColoredVertexArray<TPos>&)>& get_cluster_center,
    const GroupAndName& prefix)
{
    std::unordered_map<OrderableFixedArray<TPos, 3>, std::list<std::shared_ptr<ColoredVertexArray<TPos>>>> clustered;
    for (const auto& cva : cvas) {
        auto center = get_cluster_center(*cva);
        clustered[make_orderable(center)].push_back(cva);
    }
    std::list<PositionAndMeshes<TPos>> result;
    size_t i = 0;
    for (const auto& [center, cvas] : clustered) {
        auto& c = result.emplace_back(center);
        for (const auto& [m, grouped_cvas] : group_meshes_by_material(cvas)) {
            auto& res = c.cvas[m.material.continuous_blending_z_order]
                .emplace_back(merge_meshes(grouped_cvas, prefix + std::to_string(i++), m.material, m.morphology));
            if (res->material.aggregate_mode != AggregateMode::NODE_OBJECT) {
                throw std::runtime_error("cluster_meshes: aggregate mode is not \"NODE_OBJECT\"");
            }
            res->material.aggregate_mode = AggregateMode::NONE;
        }
    }
    return result;
}

template std::list<PositionAndMeshes<float>> VA::cluster_meshes<float>(
    const std::list<std::shared_ptr<ColoredVertexArray<float>>>& cvas,
    const std::function<FixedArray<float, 3>(const ColoredVertexArray<float>&)>& get_cluster_id,
    const GroupAndName& prefix);

template std::list<PositionAndMeshes<CompressedScenePos>> VA::cluster_meshes<CompressedScenePos>(
    const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
    const std::function<FixedArray<CompressedScenePos, 3>(const ColoredVertexArray<CompressedScenePos>&)>& get_cluster_id,
    const GroupAndName& prefix);
