// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Normal_On_Capsule.hpp"
#include <VehicleArena/Math/Fixed_Cholesky.hpp>
#include <VehicleArena/Math/Fixed_Inverse.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>

using namespace VA;

TransformationMatrix<float, ScenePos, 3> inv_(const TransformationMatrix<float, ScenePos, 3>& m) {
    auto i = inv(m.affine());
    if (!i.has_value()) {
        throw std::runtime_error("Could not invert surface normal matrix");
    }
    return { R3_from_4x4(*i).casted<float>(), t3_from_4x4(*i) };
}

NormalOnCapsule::NormalOnCapsule(
    const RigidBodyPulses& rbp,
    const TransformationMatrix<float, ScenePos, 3>& trafo)
    : rbp_{ rbp }
    , itrafo_{ inv_(trafo) }
    , rotate_{ fixed_inverse_3x3(trafo.R.T()) }
{}

NormalOnCapsule::~NormalOnCapsule() = default;

std::optional<FixedArray<float, 3>> NormalOnCapsule::get_surface_normal(
    const CollisionRidgeSphere<CompressedScenePos>& ridge,
    const FixedArray<ScenePos, 3>& position) const
{
    return get_surface_normal(position);
}

std::optional<FixedArray<float, 3>> NormalOnCapsule::get_surface_normal(
    const CollisionPolygonSphere<CompressedScenePos, 3>& triangle,
    const FixedArray<ScenePos, 3>& position) const
{
    return get_surface_normal(position);
}

std::optional<FixedArray<float, 3>> NormalOnCapsule::get_surface_normal(
    const CollisionPolygonSphere<CompressedScenePos, 4>& quad,
    const FixedArray<ScenePos, 3>& position) const
{
    return get_surface_normal(position);
}

std::optional<FixedArray<float, 3>> NormalOnCapsule::get_surface_normal(
    const FixedArray<ScenePos, 3>& position) const
{
    auto trafo = rbp_.abs_transformation();
    auto n = itrafo_.transform(trafo.itransform(position)).casted<float>();
    if (n(1) > 1) {
        n(1) -= 1;
    } else if (n(1) < -1) {
        n(1) += 1;
    } else {
        n(1) = 0.f;
    }
    n = trafo.rotate(dot1d(rotate_, n));
    auto nl = std::sqrt(sum(squared(n)));
    if (nl < 1e-12) {
        throw std::runtime_error("Could not calculate surface normal for capsule");
    }
    n /= nl;
    return n;
}
