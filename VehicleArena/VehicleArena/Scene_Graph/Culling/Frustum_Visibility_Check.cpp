// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Frustum_Visibility_Check.hpp"
#include <VehicleArena/Scene_Graph/Culling/Is_Visible.hpp>
#include <VehicleArena/Scene_Graph/Culling/Visibility_Check.hpp>

using namespace VA;

template <class TData>
FrustumVisibilityCheck<TData>::FrustumVisibilityCheck(const VisibilityCheck<TData>& vc)
    : vc_{ vc }
    , frustum_{ Frustum3<TData>::from_projection_matrix(vc.mvp()) }
{}

template <class TData>
bool FrustumVisibilityCheck<TData>::is_visible(
    const std::string& object_name,
    const Material& material,
    const Morphology& morphology,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    ExternalRenderPassType external_render_pass,
    const ExtremalAxisAlignedBoundingBox<TData, 3>& aabb) const
{
    return VA::is_visible<TData>(vc_, object_name, material, morphology, billboard_id, scene_graph_config, external_render_pass, &frustum_, &aabb);
}

namespace VA {
    template class FrustumVisibilityCheck<float>;
    template class FrustumVisibilityCheck<double>;
}
