// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Frustum_Camera_Config.hpp"
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>

using namespace VA;

FrustumCameraConfig FrustumCameraConfig::from_sensor_aabb(
    const AxisAlignedBoundingBox<float, 2>& sensor_aabb,
    float near_plane,
    float far_plane)
{
    return FrustumCameraConfig{
        .near_plane = near_plane,
        .far_plane = far_plane,
        .left = sensor_aabb.min(0) * near_plane,
        .right = sensor_aabb.max(0) * near_plane,
        .bottom = sensor_aabb.min(1) * near_plane,
        .top = sensor_aabb.max(1) * near_plane};
}
