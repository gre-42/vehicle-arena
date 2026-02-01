// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Billboard_Id.hpp>
#include <cstddef>
#include <cstdint>
#include <string>

namespace VA {

template <class TData>
class VisibilityCheck;
struct Material;
struct Morphology;
struct SceneGraphConfig;
enum class ExternalRenderPassType;
template <class TData, size_t tndim>
class ExtremalAxisAlignedBoundingBox;
template <class TData>
class Frustum3;

template <class TData>
class FrustumVisibilityCheck {
public:
    explicit FrustumVisibilityCheck(const VisibilityCheck<TData>& vc);
    bool is_visible(
        const std::string& object_name,
        const Material& material,
        const Morphology& morphology,
        BillboardId billboard_id,
        const SceneGraphConfig& scene_graph_config,
        ExternalRenderPassType external_render_pass,
        const ExtremalAxisAlignedBoundingBox<TData, 3>& aabb) const;
private:
    const VisibilityCheck<TData>& vc_;
    Frustum3<TData> frustum_;
};

}
