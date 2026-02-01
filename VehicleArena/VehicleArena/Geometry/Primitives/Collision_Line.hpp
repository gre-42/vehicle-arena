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

#ifdef __GNUC__
    #pragma GCC push_options
    #pragma GCC optimize ("O3")
#endif

namespace VA {

enum class PhysicsMaterial: uint32_t;

template <class TPosition>
struct CollisionLineSphere {
    BoundingSphere<TPosition, 3> bounding_sphere;
    PhysicsMaterial physics_material;
    FixedArray<TPosition, 2, 3> line;
    RaySegment3D<SceneDir, TPosition> ray;
    CollisionLineSphere transformed(
        const TransformationMatrix<SceneDir, ScenePos, 3>& transformation_matrix) const
    {
        return {
            bounding_sphere.transformed(transformation_matrix),
            physics_material,
            transformation_matrix.transform(line.template casted<ScenePos>()).template casted<TPosition>(),
            ray.transformed(transformation_matrix),
        };
    }
    template <class TPosition2>
    inline CollisionLineSphere<TPosition2> casted() const {
        return {
            bounding_sphere.template casted<TPosition2>(),
            physics_material,
            line.template casted<TPosition2>(),
            ray.template casted<SceneDir, TPosition2>()
        };
    }
    bool operator == (const CollisionLineSphere& other) const {
        return (bounding_sphere == other.bounding_sphere) &&
               (physics_material == other.physics_material) &&
               all(line == other.line) &&
               (ray == other.ray);
    }
};

template <class TPosition>
CollisionLineSphere<TPosition> operator + (
    const CollisionLineSphere<TPosition>& clp,
    const FixedArray<TPosition, 3>& p)
{
    return {
        clp.bounding_sphere + p,
        clp.physics_material,
        FixedArray<TPosition, 2, 3>{ clp.line[0] + p, clp.line[1] + p },
        clp.ray + p
    };
}

template <class TPosition>
CollisionLineSphere<TPosition> operator - (
    const CollisionLineSphere<TPosition>& clp,
    const FixedArray<TPosition, 3>& p)
{
    return clp + (-p);
}

template <class TPosition>
struct CollisionLineAabb {
    CollisionLineSphere<TPosition> base;
    AxisAlignedBoundingBox<TPosition, 3> aabb;
};

}

#ifdef __GNUC__
    #pragma GCC pop_options
#endif
