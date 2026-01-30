// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

namespace VA {

template <class TPos, size_t tndim>
class BoundingSphere;

struct GlLookatBoundingSphere {
    TransformationMatrix<float, ScenePos, 3> camera_model_matrix;
    float near_plane;
    float far_plane;
};

std::optional<GlLookatBoundingSphere> gl_lookat_bounding_sphere(
    float fov,
    const TransformationMatrix<float, ScenePos, 3>& observer_model_matrix,
    const BoundingSphere<ScenePos, 3>& observed_bounding_sphere);

}
