// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Group_Meshes_By_Material.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Material.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Modifiers/Material_And_Morphology.hpp>

using namespace VA;

template <class TPos>
std::map<MaterialAndMorphology, std::list<std::shared_ptr<ColoredVertexArray<TPos>>>> VA::group_meshes_by_material(
    const std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas)
{
    std::map<MaterialAndMorphology, std::list<std::shared_ptr<ColoredVertexArray<TPos>>>> result;
    for (const auto& cva : cvas) {
        result[{cva->material, cva->morphology}].push_back(cva);
    }
    return result;
}

template std::map<MaterialAndMorphology, std::list<std::shared_ptr<ColoredVertexArray<float>>>>
    VA::group_meshes_by_material(const std::list<std::shared_ptr<ColoredVertexArray<float>>>& cvas);
template std::map<MaterialAndMorphology, std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>>
    VA::group_meshes_by_material(const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas);
