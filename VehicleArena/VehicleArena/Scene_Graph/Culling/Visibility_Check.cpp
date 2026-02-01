// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Visibility_Check.hpp"
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Geometry/Material.hpp>
#include <VehicleArena/Geometry/Material/Render_Pass.hpp>
#include <VehicleArena/Geometry/Primitives/Frustum3.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Scene_Config/Scene_Graph_Config.hpp>
#include <VehicleArena/Scene_Graph/Culling/Is_Visible.hpp>
#include <VehicleArena/Testing/Assert.hpp>
#include <stdexcept>

using namespace VA;

template <class TData>
VisibilityCheck<TData>::VisibilityCheck(const FixedArray<TData, 4, 4>& mvp)
    : mvp_{ mvp }
    , orthographic_{ (mvp(3, 0) == 0 && mvp(3, 1) == 0 && mvp(3, 2) == 0 && mvp(3, 3) == 1) }
{}

template <class TData>
bool VisibilityCheck<TData>::is_visible(
    const std::string& object_name,
    const Material& material,
    const Morphology& morphology,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    ExternalRenderPassType external_render_pass) const
{
    return VA::is_visible<TData>(*this, object_name, material, morphology, billboard_id, scene_graph_config, external_render_pass, nullptr, nullptr);
}

template <class TData>
bool VisibilityCheck<TData>::is_visible(TData max_center_distance) const
{
    if (orthographic_) {
        return true;
    }
    return (distance_squared() <= squared(max_center_distance)); 
}

template <class TData>
bool VisibilityCheck<TData>::black_is_visible(
    const std::string& object_name,
    const Material& material,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    ExternalRenderPassType external_render_pass) const
{
    if ((billboard_id == BILLBOARD_ID_NONE) != material.billboard_atlas_instances.empty()) {
        throw std::runtime_error(
            "Material " + material.identifier() +
            " (1): Billboard id = " + std::to_string(billboard_id) +
            ", atlas size = " + std::to_string(material.billboard_atlas_instances.size()));
    }
    if ((external_render_pass == ExternalRenderPassType::LIGHTMAP_GLOBAL_STATIC) ||
        (external_render_pass == ExternalRenderPassType::LIGHTMAP_BLACK_GLOBAL_STATIC) ||
        (external_render_pass == ExternalRenderPassType::DIRTMAP))
    {
        return false;
    }
    if ((external_render_pass != ExternalRenderPassType::LIGHTMAP_BLOBS) &&
        (external_render_pass != ExternalRenderPassType::LIGHTMAP_BLACK_LOCAL_INSTANCES))
    {
        throw std::runtime_error("VisibilityCheck::black_is_visible: unsupported render pass: " + external_render_pass_type_to_string(external_render_pass));
    }
    ExternalRenderPassType occluder_pass = (billboard_id != BILLBOARD_ID_NONE)
        ? material.billboard_atlas_instance(billboard_id, object_name).occluder_pass
        : material.occluder_pass;
    if ((occluder_pass & external_render_pass) != external_render_pass) {
        return false;
    }
    if (orthographic_) {
        return true;
    }
    return (distance_squared() <= squared(scene_graph_config.max_distance_black));
}

template <class TData>
TData VisibilityCheck<TData>::sorting_key(const Material& m) const {
    // (mvp_ * [0; 0; 0; 1])[3] = depth in clip-space,
    // ranging from -1 to +1.
    return -std::abs(mvp_(2, 3) + (TData)1);
}

template <class TData>
bool VisibilityCheck<TData>::orthographic() const {
    return orthographic_;
}

template <class TData>
const FixedArray<TData, 4, 4>& VisibilityCheck<TData>::mvp() const {
    return mvp_;
}

template <class TData>
TData VisibilityCheck<TData>::distance_squared() const {
    if (orthographic_) {
        throw std::runtime_error("\"distance_squared()\" called on orthogonal camera");
    }
    // (mvp_ * [0; 0; 0; 1])[0..2] = position relative to camera,
    // before dividing by the affine part.
    return sum(squared(t3_from_4x4(mvp_)));
}

namespace VA {
    template class VisibilityCheck<float>;
    template class VisibilityCheck<double>;
}
