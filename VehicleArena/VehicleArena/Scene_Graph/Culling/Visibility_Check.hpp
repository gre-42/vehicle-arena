// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Billboard_Id.hpp>
#include <VehicleArena/Geometry/Primitives/Frustum3.hpp>
#include <cmath>
#include <cstdint>

namespace VA {

template <class TData, size_t tndim>
class AxisAlignedBoundingBox;
struct Material;
struct Morphology;
struct SceneGraphConfig;
struct ExternalRenderPass;
enum class ExternalRenderPassType;

template <class TData>
class VisibilityCheck {
public:
    explicit VisibilityCheck(const FixedArray<TData, 4, 4>& mvp);
    bool is_visible(
        const std::string& object_name,
        const Material& material,
        const Morphology& morphology,
        BillboardId billboard_id,
        const SceneGraphConfig& scene_graph_config,
        ExternalRenderPassType external_render_pass) const;
    bool is_visible(TData max_center_distance) const;
    bool black_is_visible(
        const std::string& object_name,
        const Material& material,
        BillboardId billboard_id,
        const SceneGraphConfig& scene_graph_config,
        ExternalRenderPassType external_render_pass) const;
    TData sorting_key(const Material& m) const;
    bool orthographic() const;
    TData distance_squared() const;
    const FixedArray<TData, 4, 4>& mvp() const;
private:
    const FixedArray<TData, 4, 4>& mvp_;
    bool orthographic_;
};

}
