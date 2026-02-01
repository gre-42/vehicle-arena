// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Billboard_Id.hpp>
#include <VehicleArena/Geometry/Primitives/Extremal_Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Extremal_Bounding_Sphere.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <list>
#include <memory>
#include <type_traits>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TData, size_t tndim>
class AxisAlignedBoundingBox;
template <class TPos, size_t tndim>
class BoundingSphere;
struct ColoredVertexArrayFilter;
template <class TPos>
class ColoredVertexArray;
struct TransformedColoredVertexArray;
struct DynamicStyle;
struct Light;
struct Skidmark;
struct RenderConfig;
struct RenderPass;
struct SceneGraphConfig;
struct AnimationState;
struct ColorStyle;
struct ExternalRenderPass;
enum class BlendingPassType;
enum class ExternalRenderPassType;
enum class PhysicsMaterial: uint32_t;
enum class RenderingStrategies;
class SceneNode;
class SmallInstancesQueues;
class LargeInstancesQueue;

class Renderable {
public:
    virtual PhysicsMaterial physics_attributes() const = 0;
    virtual RenderingStrategies rendering_strategies() const = 0;
    virtual bool requires_render_pass(ExternalRenderPassType render_pass) const = 0;
    virtual BlendingPassType required_blending_passes(ExternalRenderPassType render_pass) const = 0;
    virtual int continuous_blending_z_order() const;
    virtual void render(
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
        const ColorStyle* color_style) const;
    virtual void append_sorted_aggregates_to_queue(
        const FixedArray<ScenePos, 4, 4>& mvp,
        const TransformationMatrix<float, ScenePos, 3>& m,
        const FixedArray<ScenePos, 3>& offset,
        const SceneGraphConfig& scene_graph_config,
        const ExternalRenderPass& external_render_pass,
        std::list<std::pair<float, std::shared_ptr<ColoredVertexArray<float>>>>& aggregate_queue) const;
    virtual void append_large_aggregates_to_queue(
        const TransformationMatrix<float, ScenePos, 3>& m,
        const FixedArray<ScenePos, 3>& offset,
        const SceneGraphConfig& scene_graph_config,
        std::list<std::shared_ptr<ColoredVertexArray<float>>>& aggregate_queue) const;
    virtual void append_physics_to_queue(
        std::list<std::shared_ptr<ColoredVertexArray<float>>>& float_queue,
        std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& double_queue) const;
    virtual void append_sorted_instances_to_queue(
        const FixedArray<ScenePos, 4, 4>& mvp,
        const TransformationMatrix<float, ScenePos, 3>& m,
        const TransformationMatrix<float, ScenePos, 3>& iv,
        const FixedArray<ScenePos, 3>& offset,
        BillboardId billboard_id,
        const SceneGraphConfig& scene_graph_config,
        SmallInstancesQueues& instances_queues) const;
    virtual void append_large_instances_to_queue(
        const FixedArray<ScenePos, 4, 4>& mvp,
        const TransformationMatrix<float, ScenePos, 3>& m,
        const FixedArray<ScenePos, 3>& offset,
        BillboardId billboard_id,
        const SceneGraphConfig& scene_graph_config,
        LargeInstancesQueue& instances_queue) const;
    virtual void extend_aabb(
        const TransformationMatrix<float, ScenePos, 3>& mv,
        ExternalRenderPassType render_pass,
        AxisAlignedBoundingBox<CompressedScenePos, 3>& aabb) const;
    virtual ExtremalAxisAlignedBoundingBox<CompressedScenePos, 3> aabb() const;
    virtual ExtremalBoundingSphere<CompressedScenePos, 3> bounding_sphere() const;
    template <class TBoundingPrimitive>
    TBoundingPrimitive bounding_primitive() const {
        if constexpr (std::is_same_v<TBoundingPrimitive, AxisAlignedBoundingBox<CompressedScenePos, 3>>) {
            return aabb();
        } else if constexpr (std::is_same_v<TBoundingPrimitive, BoundingSphere<CompressedScenePos, 3>>) {
            return bounding_sphere();
        } else {
            static_assert(sizeof(TBoundingPrimitive) == 0, "Unknown bounding primitive");
        }
    }
    virtual ScenePos max_center_distance2(BillboardId billboard_id) const;
};

}
