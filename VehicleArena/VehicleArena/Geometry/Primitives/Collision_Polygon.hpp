// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Bounding_Sphere.hpp>
#include <VehicleArena/Geometry/Primitives/Convex_Polygon.hpp>
#include <VehicleArena/Geometry/Primitives/Plane_Nd.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstdint>

#ifdef __GNUC__
    #pragma GCC push_options
    #pragma GCC optimize ("O3")
#endif

namespace VA {

enum class PhysicsMaterial: uint32_t;

template <class TPosition, size_t tnvertices>
struct CollisionPolygonSphere {
    BoundingSphere<TPosition, 3> bounding_sphere;
    ConvexPolygon3D<SceneDir, TPosition, tnvertices> polygon;
    PhysicsMaterial physics_material;
    FixedArray<TPosition, tnvertices, 3> corners;
    inline CollisionPolygonSphere<TPosition, tnvertices> operator - () const {
        return {
            .bounding_sphere = bounding_sphere,
            .polygon = -polygon,
            .physics_material = physics_material,
            .corners = corners
        };
    }
    CollisionPolygonSphere<TPosition, tnvertices> transformed(
        const TransformationMatrix<SceneDir, ScenePos, 3>& transformation_matrix) const
    {
        return {
            .bounding_sphere = bounding_sphere.transformed(transformation_matrix),
            .polygon = polygon
                .template casted<SceneDir, ScenePos>()
                .transformed(transformation_matrix)
                .template casted<SceneDir, TPosition>(),
            .physics_material = physics_material,
            .corners = transformation_matrix
                .transform(corners.template casted<ScenePos>())
                .template casted<TPosition>()
        };
    }
    template <class TResult>
    inline CollisionPolygonSphere<TResult, tnvertices> casted() const {
        return {
            bounding_sphere.template casted<TResult>(),
            polygon.template casted<SceneDir, TResult>(),
            physics_material,
            corners.template casted<TResult>()
        };
    }
    bool operator == (const CollisionPolygonSphere& other) const {
        return (bounding_sphere == other.bounding_sphere) &&
               (polygon == other.polygon) &&
               (physics_material == other.physics_material) &&
               all(corners == other.corners);
    }
};

template <class TPosition, size_t tnvertices>
CollisionPolygonSphere<TPosition, tnvertices>
    operator + (
        const CollisionPolygonSphere<TPosition, tnvertices>& a,
        const FixedArray<TPosition, 3>& d)
{
    auto corners = a.corners;
    for (size_t r = 0; r < tnvertices; ++r) {
        corners[r] += d;
    }
    return {
        a.bounding_sphere + d,
        a.polygon + d,
        a.physics_material,
        corners};
}

template <class TPosition, size_t tnvertices>
CollisionPolygonSphere<TPosition, tnvertices>
    operator - (
        const CollisionPolygonSphere<TPosition, tnvertices>& a,
        const FixedArray<TPosition, 3>& d)
{
    return a + (-d);
}

template <class TPosition, size_t tnvertices>
struct CollisionPolygonAabb {
    CollisionPolygonSphere<TPosition, tnvertices> base;
    AxisAlignedBoundingBox<TPosition, 3> aabb;
};

}

#ifdef __GNUC__
    #pragma GCC pop_options
#endif
