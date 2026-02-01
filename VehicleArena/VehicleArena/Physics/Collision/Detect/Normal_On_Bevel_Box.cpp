// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Normal_On_Bevel_Box.hpp"
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>

using namespace VA;

NormalOnBevelBox::NormalOnBevelBox(
    const RigidBodyPulses& rbp,
    const AxisAlignedBoundingBox<float, 3>& aabb,
    float radius)
    : rbp_{ rbp }
    , aabb_{ aabb }
    , radius_{ radius }
{
    if (any(aabb_.size() < radius_)) {
        throw std::runtime_error("Radius too large for the given AABB");
    }
}

NormalOnBevelBox::~NormalOnBevelBox() = default;

std::optional<FixedArray<float, 3>> NormalOnBevelBox::get_surface_normal(
    const CollisionRidgeSphere<CompressedScenePos>& ridge,
    const FixedArray<ScenePos, 3>& position) const
{
    return get_surface_normal(position);
}

std::optional<FixedArray<float, 3>> NormalOnBevelBox::get_surface_normal(
    const CollisionPolygonSphere<CompressedScenePos, 3>& triangle,
    const FixedArray<ScenePos, 3>& position) const
{
    return get_surface_normal(position);
}

std::optional<FixedArray<float, 3>> NormalOnBevelBox::get_surface_normal(
    const CollisionPolygonSphere<CompressedScenePos, 4>& quad,
    const FixedArray<ScenePos, 3>& position) const
{
    return get_surface_normal(position);
}

std::optional<FixedArray<float, 3>> NormalOnBevelBox::get_surface_normal(
    const FixedArray<ScenePos, 3>& position) const
{
    auto trafo = rbp_.abs_transformation();
    auto rel_pos = trafo.itransform(position).casted<float>();
    auto dmax = rel_pos - (aabb_.max - radius_);
    auto dmin = rel_pos - (aabb_.min + radius_);
    auto sign = (dmax > -1e-6f).casted<float>() - (dmin < 1e-6f).casted<float>();
    if (sum(abs(sign)) < 2.f) {
        return std::nullopt;
    }
    auto n = maximum(dmax, -dmin) * sign;
    auto nl = std::sqrt(sum(squared(n)));
    if (nl < 1e-12f) {
        throw std::runtime_error("Could not calculate surface normal for bevel box");
    }
    n /= nl;
    return trafo.rotate(n);
}
