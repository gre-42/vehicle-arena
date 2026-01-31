// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Gl_Look_At_Bounding_Sphere.hpp"
#include <VehicleArena/Geometry/Cameras/Distance_For_Fov.hpp>
#include <VehicleArena/Geometry/Coordinates/Gl_Look_At.hpp>
#include <VehicleArena/Geometry/Primitives/Bounding_Sphere.hpp>

using namespace VA;

std::optional<GlLookatBoundingSphere> VA::gl_lookat_bounding_sphere(
    float fov,
    const TransformationMatrix<float, ScenePos, 3>& observer_model_matrix,
    const BoundingSphere<ScenePos, 3>& observed_bounding_sphere)
{
    if (observed_bounding_sphere.radius < ScenePos(1e-6)) {
        return std::nullopt;
    }
    GlLookatBoundingSphere result{
        .camera_model_matrix = uninitialized
    };
    auto dir = observed_bounding_sphere.center - observer_model_matrix.t;
    auto l2 = sum(squared(dir));
    if (l2 < 1e-12) {
        return std::nullopt;
    }
    dir /= std::sqrt(l2);
    auto dist_camera_to_observed = distance_for_fov((ScenePos)fov, observed_bounding_sphere.radius);
    result.camera_model_matrix.t = observed_bounding_sphere.center - dir * dist_camera_to_observed;
    auto lookat = gl_lookat_relative(dir.casted<float>());
    if (!lookat.has_value()) {
        return std::nullopt;
    }
    result.camera_model_matrix.R = *lookat;
    result.near_plane = (float)(dist_camera_to_observed - observed_bounding_sphere.radius);
    result.far_plane = (float)(dist_camera_to_observed + observed_bounding_sphere.radius);
    return result;
}
