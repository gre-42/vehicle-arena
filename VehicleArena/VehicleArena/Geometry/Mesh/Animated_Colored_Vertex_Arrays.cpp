// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Animated_Colored_Vertex_Arrays.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Mesh/Barrier_Triangle_Hitbox.hpp>
#include <VehicleArena/Geometry/Mesh/Bone.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array_Filter.hpp>
#include <VehicleArena/Geometry/Mesh/Modifiers/Smoothen_Edges.hpp>
#include <VehicleArena/Geometry/Mesh/Smoothness_Target.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Transformation/Quaternion.hpp>
#include <VehicleArena/Testing/Assert.hpp>

using namespace VA;

AnimatedColoredVertexArrays::AnimatedColoredVertexArrays()
    : bone_indices{ "Bone index" }
{}

AnimatedColoredVertexArrays::AnimatedColoredVertexArrays(
    const AnimatedColoredVertexArrays& other,
    const ColoredVertexArrayFilter& filter)
    : skeleton{ other.skeleton }
    , bone_indices{ other.bone_indices }
{
    for (const auto& cva : other.scvas) {
        if (filter.matches(*cva)) {
            scvas.push_back(cva);
        }
    }
    for (const auto& cva : other.dcvas) {
        if (filter.matches(*cva)) {
            dcvas.push_back(cva);
        }
    }
}

AnimatedColoredVertexArrays::~AnimatedColoredVertexArrays() = default;

void AnimatedColoredVertexArrays::insert(const AnimatedColoredVertexArrays& other) {
    if (!bone_indices.empty() || !other.bone_indices.empty()) {
        throw std::runtime_error("Merging AnimatedColoredVertexArrays does not support bone indices");
    }
    if ((skeleton != nullptr) || (other.skeleton != nullptr)) {
        throw std::runtime_error("Merging AnimatedColoredVertexArrays does not support skeleton");
    }
    scvas.insert(scvas.end(), other.scvas.begin(), other.scvas.end());
    dcvas.insert(dcvas.end(), other.dcvas.begin(), other.dcvas.end());
}

UUVector<OffsetAndQuaternion<float, float>> AnimatedColoredVertexArrays::vectorize_joint_poses(
    const StringWithHashUnorderedMap<OffsetAndQuaternion<float, float>>& poses) const
{
    UUVector<OffsetAndQuaternion<float, float>> ms(bone_indices.size());
#ifndef NDEBUG
    for (auto& m : ms) {
        m.t = fixed_nans<float, 3>();
    }
#endif
    for (const auto& [name, pose] : poses) {
        auto& it = bone_indices.get(name);
        ms.at(it) = pose;
    }
#ifndef NDEBUG
    for (const auto& m : ms) {
        if (any(VA::isnan(m.t))) {
            throw std::runtime_error("Pose contains NAN values or was not set");
        }
    }
#endif
    return ms;
}

std::shared_ptr<AnimatedColoredVertexArrays> AnimatedColoredVertexArrays::generate_grind_lines(
    float edge_angle,
    float averaged_normal_angle,
    const ColoredVertexArrayFilter& filter) const
{
    auto result = std::make_shared<AnimatedColoredVertexArrays>();
    for (const auto& t : scvas) {
        if (filter.matches(*t)) {
            result->scvas.push_back(
                t->generate_grind_lines(edge_angle, averaged_normal_angle));
        }
    }
    for (const auto& t : dcvas) {
        if (filter.matches(*t)) {
            result->dcvas.push_back(
                t->generate_grind_lines(edge_angle, averaged_normal_angle));
        }
    }
    return result;
}

void AnimatedColoredVertexArrays::create_barrier_triangle_hitboxes(
    float depth,
    PhysicsMaterial destination_physics_material,
    const ColoredVertexArrayFilter& filter)
{
    for (const auto& t : scvas) {
        if (filter.matches(*t)) {
            throw std::runtime_error("Single-precision array matches terrain convex decomposition filter");
        }
    }
    std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>> new_dvcas;
    for (const auto& cva : dcvas) {
        if (filter.matches(*cva)) {
            for (const auto& cva : VA::create_barrier_triangle_hitboxes(
                *cva,
                depth / 2.f,
                destination_physics_material))
            {
                new_dvcas.push_back(cva);
            }
        } else {
            new_dvcas.push_back(cva);
        }
    }
    dcvas = std::move(new_dvcas);
}

void AnimatedColoredVertexArrays::smoothen_edges(
    SmoothnessTarget target,
    float smoothness,
    size_t niterations,
    float decay)
{
    if (target == SmoothnessTarget::PHYSICS) {
        std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>> new_dvcas;
        for (const auto& l : dcvas) {
            if (!any(l->morphology.physics_material & PhysicsMaterial::ATTR_COLLIDE)) {
                continue;
            }
            new_dvcas.emplace_back(std::make_shared<ColoredVertexArray<CompressedScenePos>>(
                l->name + "_smooth",
                l->material,
                l->morphology - PhysicsMaterial::ATTR_VISIBLE,
                l->modifier_backlog,
                UUVector<FixedArray<ColoredVertex<CompressedScenePos>, 4>>{},
                UUVector<FixedArray<ColoredVertex<CompressedScenePos>, 3>>{l->triangles},
                UUVector<FixedArray<ColoredVertex<CompressedScenePos>, 2>>{},
                UUVector<FixedArray<std::vector<BoneWeight>, 3>>{},
                UUVector<FixedArray<float, 3>>{},
                UUVector<FixedArray<uint8_t, 3>>{},
                std::vector<UUVector<FixedArray<float, 3, 2>>>{},
                std::vector<UUVector<FixedArray<float, 3>>>{},
                UUVector<FixedArray<float, 3>>{},
                UUVector<FixedArray<float, 4>>{}));
            l->morphology.physics_material &= ~PhysicsMaterial::ATTR_COLLIDE;
        }
        VA::smoothen_edges(new_dvcas, {}, smoothness, niterations, decay);
        dcvas.insert(dcvas.end(), new_dvcas.begin(), new_dvcas.end());
    } else {
        throw std::runtime_error("Only physics smoothness-target is implemented");
    }
}

std::shared_ptr<AnimatedColoredVertexArrays> AnimatedColoredVertexArrays::triangulate(
    RectangleTriangulationMode mode,
    DelaunayErrorBehavior error_behavior) const
{
    auto result = std::make_shared<AnimatedColoredVertexArrays>();
    for (const auto& cva : scvas) {
        result->scvas.push_back(cva->triangulate(mode, error_behavior));
    }
    for (const auto& cva : dcvas) {
        result->dcvas.push_back(cva->triangulate(mode, error_behavior));
    }
    return result;
}

void AnimatedColoredVertexArrays::check_consistency() const {
    assert_true(bone_indices.empty() == !skeleton);
    auto validate_cva = [this](const auto& cvas){
        for (const auto& cva : cvas) {
            assert_true(cva->triangle_bone_weights.empty() == !skeleton);
            if (!cva->triangle_bone_weights.empty()) {
                assert_true(cva->triangle_bone_weights.size() == cva->triangles.size());
            }
            if (!cva->continuous_triangle_texture_layers.empty()) {
                assert_true(cva->continuous_triangle_texture_layers.size() == cva->triangles.size());
            }
            if (!cva->discrete_triangle_texture_layers.empty()) {
                assert_true(cva->discrete_triangle_texture_layers.size() == cva->triangles.size());
            }
        }
    };
    validate_cva(scvas);
    validate_cva(dcvas);
}

template <class TPos>
std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& AnimatedColoredVertexArrays::cvas() {
    if constexpr (std::is_same_v<TPos, float>) {
        return scvas;
    } else if constexpr (std::is_same_v<TPos, CompressedScenePos>) {
        return dcvas;
    } else {
        throw std::runtime_error("Unknown mesh precision");
    }
}

void AnimatedColoredVertexArrays::print(std::ostream& ostr) const {
    ostr << "AnimatedColoredVertexArrays\n";
    for (const auto& cva : scvas) {
        cva->print(ostr);
    }
    for (const auto& cva : dcvas) {
        cva->print(ostr);
    }
}

template std::list<std::shared_ptr<ColoredVertexArray<float>>>& AnimatedColoredVertexArrays::cvas();
template std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& AnimatedColoredVertexArrays::cvas();
