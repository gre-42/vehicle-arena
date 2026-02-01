// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Modifiers/Point_To_Grid_Center.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <functional>
#include <list>
#include <memory>

namespace VA {

class GroupAndName;
template <class TPos>
struct PositionAndMeshes;

template <class TPos>
std::list<PositionAndMeshes<TPos>> cluster_meshes(
    const std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas,
    const std::function<FixedArray<TPos, 3>(const ColoredVertexArray<TPos>&)>& get_cluster_center,
    const GroupAndName& prefix);

template <class TPos, class TWidth>
inline std::function<FixedArray<TPos, 3>(const ColoredVertexArray<TPos>&)> cva_to_grid_center(
    const FixedArray<TWidth, 3>& width)
{
    return [width](const ColoredVertexArray<TPos>& cva){
        if (any(width == (TWidth)0.f)) {
            throw std::runtime_error("Cluster width is zero (0)");
        }
        return point_to_grid_center(cva.aabb().data().center(), width);
    };
}

template <class TPos>
std::unordered_map<float, std::list<std::shared_ptr<ColoredVertexArray<TPos>>>>
    object_cluster_width_groups(const std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas)
{
    std::unordered_map<float, std::list<std::shared_ptr<ColoredVertexArray<TPos>>>> result;
    for (const auto& cva : cvas) {
        result[cva->morphology.object_cluster_width].push_back(cva);
    }
    return result;
}

}
