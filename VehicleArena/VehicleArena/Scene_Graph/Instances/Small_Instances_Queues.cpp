// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Small_Instances_Queues.hpp"
#include <VehicleArena/Geometry/Material/Render_Pass.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Transformed_Colored_Vertex_Array.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Scene_Graph/Culling/Visibility_Check.hpp>
#include <VehicleArena/Testing/Assert.hpp>

using namespace VA;

SmallInstancesQueues::SmallInstancesQueues(
    ExternalRenderPassType main_render_pass,
    const std::set<ExternalRenderPassType>& black_render_passes)
: main_render_pass_{main_render_pass}
{
    for (const auto& r : black_render_passes) {
        assert_true(!any(r & ExternalRenderPassType::STANDARD_MASK));
        black_queues_[r];
    }
}

SmallInstancesQueues::~SmallInstancesQueues()
{}

void SmallInstancesQueues::insert(
    const std::list<std::shared_ptr<ColoredVertexArray<float>>>& scvas,
    const FixedArray<ScenePos, 4, 4>& mvp,
    const TransformationMatrix<float, ScenePos, 3>& m,
    const FixedArray<ScenePos, 3>& offset,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config)
{
    TransformationMatrix<float, float, 3> m_shifted{m.R, (m.t - offset).casted<float>()};
    VisibilityCheck vc{ mvp };
    for (const auto& scva : scvas) {
        if (vc.is_visible(scva->name.full_name(), scva->material, scva->morphology, billboard_id, scene_graph_config, main_render_pass_))
        {
            TransformedColoredVertexArray* tcva;
            if (scva->material.blend_mode == BlendMode::INVISIBLE) {
                tcva = &invisible_queue_.emplace_back(TransformedColoredVertexArray{
                    .scva = scva,
                    .trafo = TransformationAndBillboardId{
                        .transformation_matrix = m_shifted,
                        .billboard_id = billboard_id}});
            } else {
                tcva = &standard_queue_.emplace_back(
                    (float)vc.sorting_key(scva->material),
                    TransformedColoredVertexArray{
                        .scva = scva,
                        .trafo = TransformationAndBillboardId{
                            .transformation_matrix = m_shifted,
                            .billboard_id = billboard_id}}).second;
            }
            for (auto& [rp, instances] : black_queues_) {
                assert_true(rp != main_render_pass_);
                if (vc.black_is_visible(
                    scva->name.full_name(),
                    scva->material,
                    billboard_id,
                    scene_graph_config,
                    rp))
                {
                    instances.push_back(tcva);
                }
            }
        }
    }
}

std::map<ExternalRenderPassType, std::list<TransformedColoredVertexArray>> SmallInstancesQueues::sorted_instances()
{
    std::map<ExternalRenderPassType, std::list<TransformedColoredVertexArray>> results;
    for (auto& [rp, lst] : black_queues_) {
        assert_true(rp != main_render_pass_);
        auto& dlst = results[rp];
        for (const auto& e : lst) {
            dlst.push_back(*e);
        }
    }
    {
        standard_queue_.sort([](auto& a, auto& b){ return a.first < b.first; });
        auto& dlst = results[main_render_pass_];
        for (auto& [_, e] : standard_queue_) {
            dlst.push_back(e);
        }
    }
    return results;
}
