// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Renderable.hpp"
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Bounding_Sphere.hpp>
#include <stdexcept>

using namespace VA;

int Renderable::continuous_blending_z_order() const {
    return 0;
}

void Renderable::render(
    const FixedArray<ScenePos, 4, 4>& mvp,
    const TransformationMatrix<float, ScenePos, 3>& m,
    const TransformationMatrix<float, ScenePos, 3>& iv,
    const DynamicStyle* dynamic_style,
    const std::list<std::pair<TransformationMatrix<float, ScenePos, 3>, std::shared_ptr<Light>>>& lights,
    const std::list<std::pair<TransformationMatrix<float, ScenePos, 3>, std::shared_ptr<Skidmark>>>& skidmarks,
    const SceneGraphConfig& scene_graph_config,
    const RenderConfig& render_config,
    const RenderPass& render_pass,
    const AnimationState* animation_state,
    const ColorStyle* color_style) const
{}

void Renderable::append_sorted_aggregates_to_queue(
    const FixedArray<ScenePos, 4, 4>& mvp,
    const TransformationMatrix<float, ScenePos, 3>& m,
    const FixedArray<ScenePos, 3>& offset,
    const SceneGraphConfig& scene_graph_config,
    const ExternalRenderPass& external_render_pass,
    std::list<std::pair<float, std::shared_ptr<ColoredVertexArray<float>>>>& aggregate_queue) const
{}

void Renderable::append_large_aggregates_to_queue(
    const TransformationMatrix<float, ScenePos, 3>& m,
    const FixedArray<ScenePos, 3>& offset,
    const SceneGraphConfig& scene_graph_config,
    std::list<std::shared_ptr<ColoredVertexArray<float>>>& aggregate_queue) const
{}

void Renderable::append_physics_to_queue(
    std::list<std::shared_ptr<ColoredVertexArray<float>>>& float_queue,
    std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& double_queue) const
{}

void Renderable::append_sorted_instances_to_queue(
    const FixedArray<ScenePos, 4, 4>& mvp,
    const TransformationMatrix<float, ScenePos, 3>& m,
    const TransformationMatrix<float, ScenePos, 3>& iv,
    const FixedArray<ScenePos, 3>& offset,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    SmallInstancesQueues& instances_queues) const
{}

void Renderable::append_large_instances_to_queue(
    const FixedArray<ScenePos, 4, 4>& mvp,
    const TransformationMatrix<float, ScenePos, 3>& m,
    const FixedArray<ScenePos, 3>& offset,
    BillboardId billboard_id,
    const SceneGraphConfig& scene_graph_config,
    LargeInstancesQueue& instances_queue) const
{}

void Renderable::extend_aabb(
    const TransformationMatrix<float, ScenePos, 3>& mv,
    ExternalRenderPassType render_pass,
    AxisAlignedBoundingBox<CompressedScenePos, 3>& aabb) const
{
    throw std::runtime_error("Renderable::extend_aabb not implemented");
}

ExtremalAxisAlignedBoundingBox<CompressedScenePos, 3> Renderable::aabb() const {
    throw std::runtime_error("Renderable::aabb not implemented");
}

ExtremalBoundingSphere<CompressedScenePos, 3> Renderable::bounding_sphere() const {
    throw std::runtime_error("Renderable::bounding_sphere not implemented");
}

ScenePos Renderable::max_center_distance2(BillboardId billboard_id) const {
    throw std::runtime_error("Renderable::max_center_distance2 not implemented");
}
