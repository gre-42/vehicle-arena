// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Large_Instances_Queue.hpp"
#include <VehicleArena/Geometry/Material/Render_Pass.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Transformed_Colored_Vertex_Array.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Scene_Graph/Culling/Visibility_Check.hpp>
#include <VehicleArena/Testing/Assert.hpp>
#include <stdexcept>

using namespace VA;

LargeInstancesQueue::LargeInstancesQueue(ExternalRenderPassType render_pass)
    : render_pass_{ render_pass }
{
    if ((render_pass_ != ExternalRenderPassType::LIGHTMAP_GLOBAL_STATIC) &&
        (render_pass_ != ExternalRenderPassType::LIGHTMAP_BLACK_GLOBAL_STATIC) &&
        (render_pass_ != ExternalRenderPassType::DIRTMAP) &&
        !any(render_pass_ & ExternalRenderPassType::STANDARD_MASK))
    {
        throw std::runtime_error("Unknown render pass");
    }
}

LargeInstancesQueue::~LargeInstancesQueue()
{}

void LargeInstancesQueue::insert(
    const std::list<std::shared_ptr<ColoredVertexArray<float>>>& scvas,
    const FixedArray<ScenePos, 4, 4>& mvp,
    const TransformationMatrix<float, ScenePos, 3>& m,
    const FixedArray<ScenePos, 3>& offset,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    InvisibilityHandling invisibility_handling)
{
    TransformationMatrix<float, float, 3> mo{m.R, (m.t - offset).casted<float>()};
    for (const auto& scva : scvas) {
        if (any(render_pass_ & ExternalRenderPassType::STANDARD_MASK)) {
            if (!VisibilityCheck{mvp}.is_visible(scva->name.full_name(), scva->material, scva->morphology, billboard_id, scene_graph_config, render_pass_)) {
                continue;
            }
        } else if (render_pass_ == ExternalRenderPassType::DIRTMAP) {
            continue;
        } else if (any(render_pass_ & ExternalRenderPassType::IS_GLOBAL_MASK)) {
            ExternalRenderPassType occluder_pass = (billboard_id != BILLBOARD_ID_NONE)
                ? scva->material.billboard_atlas_instance(billboard_id, scva->name.full_name()).occluder_pass
                : scva->material.occluder_pass;
            if (!any(occluder_pass & ExternalRenderPassType::IS_GLOBAL_MASK)) {
                if (invisibility_handling == InvisibilityHandling::SKIP) {
                    continue;
                } else {
                    throw std::runtime_error("Static instance has no occluder pass: \"" + scva->name.full_name() + '"');
                }
            }
            if ((occluder_pass & render_pass_) != render_pass_) {
                continue;
            }
        } else {
            throw std::runtime_error("Unsupported render pass: " + external_render_pass_type_to_string(render_pass_));
        }
        queue_.push_back(TransformedColoredVertexArray{
            .scva = scva,
            .trafo = TransformationAndBillboardId{
                .transformation_matrix = mo,
                .billboard_id = billboard_id}});
    }
}

const std::list<TransformedColoredVertexArray>& LargeInstancesQueue::queue() const {
    return queue_;
}

ExternalRenderPassType LargeInstancesQueue::render_pass() const {
    return render_pass_;
}
