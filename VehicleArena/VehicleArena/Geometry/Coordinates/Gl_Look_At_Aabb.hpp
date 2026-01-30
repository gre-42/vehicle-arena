// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Intersection/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

namespace VA {

template <class TData, size_t tndim>
class AxisAlignedBoundingBox;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;

struct GlLookatAabb {
    FixedArray<float, 3, 3> extrinsic_R;
    AxisAlignedBoundingBox<float, 2> sensor_aabb;
    float near_plane;
    float far_plane;
};

std::optional<GlLookatAabb> gl_lookat_aabb(
    const FixedArray<ScenePos, 3>& camera_position,
    const TransformationMatrix<float, ScenePos, 3>& object_model_matrix,
    const AxisAlignedBoundingBox<ScenePos, 3>& object_aabb);

}
