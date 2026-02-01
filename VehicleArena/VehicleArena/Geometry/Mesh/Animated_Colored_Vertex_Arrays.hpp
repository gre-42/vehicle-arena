// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Initialization/Default_Uninitialized_Vector.hpp>
#include <VehicleArena/Map/String_With_Hash_Unordered_Map.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <list>
#include <map>
#include <memory>
#include <string>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
template <class TDir, class TPos>
class OffsetAndQuaternion;
struct Bone;
template <class TPos>
class ColoredVertexArray;
struct ColoredVertexArrayFilter;
enum class PhysicsMaterial: uint32_t;
enum class SmoothnessTarget;
enum class RectangleTriangulationMode;
enum class DelaunayErrorBehavior;

struct AnimatedColoredVertexArrays {
    AnimatedColoredVertexArrays();
    AnimatedColoredVertexArrays(
        const AnimatedColoredVertexArrays& other,
        const ColoredVertexArrayFilter& filter);
    ~AnimatedColoredVertexArrays();
    std::shared_ptr<Bone> skeleton;
    StringWithHashUnorderedMap<size_t> bone_indices;
    std::list<std::shared_ptr<ColoredVertexArray<float>>> scvas;
    std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>> dcvas;
    void insert(const AnimatedColoredVertexArrays& other);
    template <class TPos>
    std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas();
    UUVector<OffsetAndQuaternion<float, float>> vectorize_joint_poses(
        const StringWithHashUnorderedMap<OffsetAndQuaternion<float, float>>& poses) const;
    std::shared_ptr<AnimatedColoredVertexArrays> generate_grind_lines(
        float edge_angle,
        float averaged_normal_angle,
        const ColoredVertexArrayFilter& filter) const;
    void create_barrier_triangle_hitboxes(
        float depth,
        PhysicsMaterial destination_physics_material,
        const ColoredVertexArrayFilter& filter);
    void smoothen_edges(
        SmoothnessTarget target,
        float smoothness,
        size_t niterations,
        float decay = 0.97f);
    std::shared_ptr<AnimatedColoredVertexArrays> triangulate(
        RectangleTriangulationMode mode,
        DelaunayErrorBehavior error_behavior) const;
    void check_consistency() const;
    void print(std::ostream& ostr) const;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(skeleton);
        archive(bone_indices.elements());
        archive(scvas);
        archive(dcvas);
    }
};

}
