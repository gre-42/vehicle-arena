// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Merge_Meshes.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Material.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <list>

using namespace VA;

template <class TPos>
std::shared_ptr<ColoredVertexArray<TPos>> VA::merge_meshes(
    const std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas,
    const GroupAndName& name,
    const Material& material,
    const Morphology& morphology)
{
    if (cvas.empty()) {
        throw std::runtime_error("Attempt to merge empty list of meshes");
    }
    std::list<FixedArray<ColoredVertex<TPos>, 3>> triangles;
    std::list<FixedArray<uint8_t, 3>> discrete_triangle_texture_layers;
    std::list<FixedArray<float, 3>> alpha;
    std::list<FixedArray<float, 4>> interiormap_uvmaps;
    for (const auto& cva : cvas) {
        if (!cva->discrete_triangle_texture_layers.empty() &&
            (cva->discrete_triangle_texture_layers.size() != cva->triangles.size()))
        {
            throw std::runtime_error("merge_meshes: discrete_triangle_texture_layers size mismatch");
        }
        if (!cva->alpha.empty() &&
            (cva->alpha.size() != cva->triangles.size()))
        {
            throw std::runtime_error("merge_meshes: alpha size mismatch");
        }
        if (!cva->interiormap_uvmaps.empty() &&
            (cva->interiormap_uvmaps.size() != cva->triangles.size()))
        {
            throw std::runtime_error("merge_meshes: interiormap_uvmaps size mismatch");
        }
        for (const auto& t : cva->triangles) {
            triangles.push_back(t);
        }
        for (const auto& t : cva->discrete_triangle_texture_layers) {
            discrete_triangle_texture_layers.push_back(t);
        }
        for (const auto& t : cva->alpha) {
            alpha.push_back(t);
        }
        for (const auto& t : cva->interiormap_uvmaps) {
            interiormap_uvmaps.push_back(t);
        }
    }
    return std::make_shared<ColoredVertexArray<TPos>>(
        name,
        material,
        morphology,
        ModifierBacklog{},
        UUVector<FixedArray<ColoredVertex<TPos>, 4>>{},
        UUVector<FixedArray<ColoredVertex<TPos>, 3>>(triangles.begin(), triangles.end()),
        UUVector<FixedArray<ColoredVertex<TPos>, 2>>{},
        UUVector<FixedArray<std::vector<BoneWeight>, 3>>{},
        UUVector<FixedArray<float, 3>>{},
        UUVector<FixedArray<uint8_t, 3>>{discrete_triangle_texture_layers.begin(), discrete_triangle_texture_layers.end()},
        std::vector<UUVector<FixedArray<float, 3, 2>>>{},
        std::vector<UUVector<FixedArray<float, 3>>>{},
        UUVector<FixedArray<float, 3>>(alpha.begin(), alpha.end()),
        UUVector<FixedArray<float, 4>>(interiormap_uvmaps.begin(), interiormap_uvmaps.end()));
}

template std::shared_ptr<ColoredVertexArray<float>> VA::merge_meshes<float>(
    const std::list<std::shared_ptr<ColoredVertexArray<float>>>& cvas,
    const GroupAndName& name,
    const Material& material,
    const Morphology& morphology);
template std::shared_ptr<ColoredVertexArray<CompressedScenePos>> VA::merge_meshes<CompressedScenePos>(
    const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
    const GroupAndName& name,
    const Material& material,
    const Morphology& morphology);
