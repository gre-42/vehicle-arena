// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Is_Visible.hpp"
#include <VehicleArena/Geometry/Material.hpp>
#include <VehicleArena/Geometry/Morphology.hpp>
#include <VehicleArena/Geometry/Primitives/Extremal_Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Scene_Config/Scene_Graph_Config.hpp>
#include <VehicleArena/Scene_Graph/Culling/Visibility_Check.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/IRenderable_Hider.hpp>
#include <VehicleArena/Testing/Assert.hpp>
#include <stdexcept>

using namespace VA;

template <class TData>
bool VA::is_visible(
    const VisibilityCheck<TData>& vc,
    const std::string& object_name,
    const Material& material,
    const Morphology& morphology,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    ExternalRenderPassType external_render_pass,
    const Frustum3<TData>* frustum,
    const ExtremalAxisAlignedBoundingBox<TData, 3>* aabb)
{
    if ((billboard_id == BILLBOARD_ID_NONE) != material.billboard_atlas_instances.empty()) {
        throw std::runtime_error(
            "Material " + material.identifier() +
            " (0): Billboard id = " + std::to_string(billboard_id) +
            ", atlas size = " + std::to_string(material.billboard_atlas_instances.size()));
    }
    if ((scene_graph_config.renderable_hider != nullptr) &&
        !scene_graph_config.renderable_hider->is_visible(object_name))
    {
        return false;
    }
    if (external_render_pass != ExternalRenderPassType::STANDARD_AND_LOCAL_LIGHTMAP)
    {
        if (any(external_render_pass & ExternalRenderPassType::LIGHTMAP_ANY_MASK) ||
            any(external_render_pass & ExternalRenderPassType::DIRTMAP_MASK))
        {
            ExternalRenderPassType occluder_pass = material.get_occluder_pass(billboard_id, object_name);
            return (occluder_pass & external_render_pass) == external_render_pass;
        }
        if (material.blend_mode == BlendMode::INVISIBLE) {
            return false;
        }
    }
    if (any(external_render_pass & ExternalRenderPassType::IMPOSTER_NODE)) {
        return morphology.center_distances2(1) == INFINITY;
    } else if (any(external_render_pass & ExternalRenderPassType::ZOOM_NODE)) {
        return morphology.center_distances2(0) == 0.f;
    } else if (any(external_render_pass & ExternalRenderPassType::STANDARD_MASK)) {
        if (vc.orthographic()) {
            return true;
        }
        TData max_center_distance2 = (TData)material.max_center_distance2(billboard_id, morphology, object_name);
        TData dist2 = vc.distance_squared();
        if (!((dist2 >= morphology.center_distances2(0)) && (dist2 < max_center_distance2))) {
            return false;
        }
        if (!frustum != !aabb) {
            throw std::runtime_error("Inconsistent frustum and AABB NAN-ness");
        }
        if (frustum == nullptr) {
            return true;
        } else {
            return aabb->full() || frustum->intersects(aabb->data());
        }
    }
    throw std::runtime_error("VisibilityCheck::is_visible received unknown render pass type");
}

template bool VA::is_visible<float>(
    const VisibilityCheck<float>& vc,
    const std::string& object_name,
    const Material& material,
    const Morphology& morphology,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    ExternalRenderPassType external_render_pass,
    const Frustum3<float>* frustum,
    const ExtremalAxisAlignedBoundingBox<float, 3>* aabb);

template bool VA::is_visible<double>(
    const VisibilityCheck<double>& vc,
    const std::string& object_name,
    const Material& material,
    const Morphology& morphology,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    ExternalRenderPassType external_render_pass,
    const Frustum3<double>* frustum,
    const ExtremalAxisAlignedBoundingBox<double, 3>* aabb);
