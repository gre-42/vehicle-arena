// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Physics/Interfaces/ISurface_Normal.hpp>

namespace VA {

class RigidBodyPulses;

class NormalOnBevelBox: public ISurfaceNormal {
public:
	NormalOnBevelBox(
        const RigidBodyPulses& rbp,
		const AxisAlignedBoundingBox<float, 3>& aabb,
        float radius);
    ~NormalOnBevelBox();
    virtual std::optional<FixedArray<float, 3>> get_surface_normal(
        const CollisionRidgeSphere<CompressedScenePos>& ridge,
        const FixedArray<ScenePos, 3>& position) const override;
    virtual std::optional<FixedArray<float, 3>> get_surface_normal(
        const CollisionPolygonSphere<CompressedScenePos, 3>& triangle,
        const FixedArray<ScenePos, 3>& position) const override;
    virtual std::optional<FixedArray<float, 3>> get_surface_normal(
        const CollisionPolygonSphere<CompressedScenePos, 4>& quad,
        const FixedArray<ScenePos, 3>& position) const override;
private:
    std::optional<FixedArray<float, 3>> get_surface_normal(
        const FixedArray<ScenePos, 3>& position) const;

    const RigidBodyPulses& rbp_;
	AxisAlignedBoundingBox<float, 3> aabb_;
    float radius_;
};

}
