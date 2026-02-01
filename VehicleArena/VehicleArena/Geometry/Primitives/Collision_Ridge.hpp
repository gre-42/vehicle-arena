// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Bounding_Sphere.hpp>
#include <VehicleArena/Geometry/Primitives/Ray_Segment_3D.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstdint>

namespace VA {

enum class PhysicsMaterial: uint32_t;
static const SceneDir RIDGE_SPECIAL_THRESHOLD = 2.f;
static const SceneDir RIDGE_SINGLE_FACE = 3.f;
static const SceneDir RIDGE_UNTOUCHABLE = 4.f;
static const SceneDir RIDGE_360 = 5.f;

enum class SingleFaceBehavior {
    TOUCHABLE,
    UNTOUCHABLE
};

template <class TPosition>
struct CollisionRidgeSphere {
    BoundingSphere<TPosition, 3> bounding_sphere;
    PhysicsMaterial physics_material;
    FixedArray<TPosition, 2, 3> edge;
    RaySegment3D<SceneDir, TPosition> ray;
    FixedArray<SceneDir, 3> normal;
    SceneDir min_cos;
    FixedArray<SceneDir, 3> tangent() const;
    bool is_touchable(SingleFaceBehavior behavior) const;
    bool is_oriented() const;
    void combine(const CollisionRidgeSphere<TPosition>& other, SceneDir max_min_cos_ridge);
    void finalize();
    CollisionRidgeSphere transformed(const TransformationMatrix<SceneDir, ScenePos, 3>& trafo) const;
    template <class TPosition2>
    inline CollisionRidgeSphere<TPosition2> casted() const {
        return CollisionRidgeSphere<TPosition2>{
            .bounding_sphere = bounding_sphere.template casted<TPosition2>(),
            .physics_material = physics_material,
            .edge = edge.template casted<TPosition2>(),
            .ray = ray.template casted<SceneDir, TPosition2>(),
            .normal = normal,
            .min_cos = min_cos
        };
    }
    bool operator == (const CollisionRidgeSphere& other) const {
        return (bounding_sphere == other.bounding_sphere) &&
               (physics_material == other.physics_material) &&
               all(edge == other.edge) &&
               (ray == other.ray) &&
               all(normal == other.normal) &&
               (min_cos == other.min_cos);
    }
};

template <class TPosition>
CollisionRidgeSphere<TPosition> operator + (
    const CollisionRidgeSphere<TPosition>& crs,
    const FixedArray<TPosition, 3>& p)
{
    return {
        crs.bounding_sphere + p,
        crs.physics_material,
        FixedArray<TPosition, 2, 3>{ crs.edge[0] + p,  crs.edge[1] + p },
        crs.ray + p,
        crs.normal,
        crs.min_cos
    };
}

}
