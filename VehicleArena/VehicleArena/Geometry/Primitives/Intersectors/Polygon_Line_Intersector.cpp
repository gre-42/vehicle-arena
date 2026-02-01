// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Polygon_Line_Intersector.hpp"
#include <VehicleArena/Geometry/Interfaces/IIntersectable.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Line.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Ridge.hpp>
#include <VehicleArena/Geometry/Primitives/Intersectors/Intersection_Info.hpp>

#ifdef __GNUC__
    #pragma GCC push_options
    #pragma GCC optimize ("O3")
#endif

using namespace VA;

// Quad - ridge
bool VA::intersect(
    const CollisionPolygonSphere<CompressedScenePos, 4>& q0,
    const CollisionRidgeSphere<CompressedScenePos>& r1,
    IntersectionInfo& intersection_info)
{
    ScenePos ray_t;
    auto q0c_polygon = q0.polygon.casted<ScenePos, ScenePos>();
    if (!r1.ray.casted<ScenePos, ScenePos>().intersects(q0c_polygon, &ray_t, &intersection_info.intersection_point)) {
        return false;
    }
    intersection_info.normal0 = q0.polygon.plane.normal;
    intersection_info.ray_t = ray_t;
    return true;
}

// Triangle - ridge
bool VA::intersect(
    const CollisionPolygonSphere<CompressedScenePos, 3>& t0,
    const CollisionRidgeSphere<CompressedScenePos>& r1,
    IntersectionInfo& intersection_info)
{
    ScenePos ray_t;
    auto t0c_polygon = t0.polygon.casted<ScenePos, ScenePos>();
    if (!r1.ray.casted<ScenePos, ScenePos>().intersects(t0c_polygon, &ray_t, &intersection_info.intersection_point)) {
        return false;
    }
    intersection_info.normal0 = t0.polygon.plane.normal;
    intersection_info.ray_t = ray_t;
    return true;
}

// Quad - line
bool VA::intersect(
    const CollisionPolygonSphere<CompressedScenePos, 4>& q0,
    const CollisionLineSphere<CompressedScenePos>& l1,
    IntersectionInfo& intersection_info)
{
    ScenePos ray_t;
    auto q0c_polygon = q0.polygon.casted<ScenePos, ScenePos>();
    if (!l1.ray.casted<ScenePos, ScenePos>().intersects(q0c_polygon, &ray_t, &intersection_info.intersection_point)) {
        return false;
    }
    intersection_info.normal0 = q0.polygon.plane.normal;
    intersection_info.ray_t = ray_t;
    return true;
}

// Triangle - line
bool VA::intersect(
    const CollisionPolygonSphere<CompressedScenePos, 3>& t0,
    const CollisionLineSphere<CompressedScenePos>& l1,
    IntersectionInfo& intersection_info)
{
    ScenePos ray_t;
    auto t0c_polygon = t0.polygon.casted<ScenePos, ScenePos>();
    if (!l1.ray.casted<ScenePos, ScenePos>().intersects(t0c_polygon, &ray_t, &intersection_info.intersection_point)) {
        return false;
    }
    intersection_info.normal0 = t0.polygon.plane.normal;
    intersection_info.ray_t = ray_t;
    return true;
}

// Quad - intersectable
bool VA::intersect(
    const CollisionPolygonSphere<CompressedScenePos, 4>& q0,
    const IIntersectable& i1,
    IntersectionInfo& intersection_info)
{
    FixedArray<SceneDir, 3> normal = uninitialized;
    ScenePos ray_t = NAN;
    ScenePos overlap;
    if (!i1.touches(q0, overlap, intersection_info.intersection_point, normal)) {
        return false;
    }
    intersection_info.normal0 = q0.polygon.plane.normal;
    intersection_info.ray_t = ray_t;
    intersection_info.no = NormalAndOverlap{ -normal, overlap };
    return true;
}

// Triangle - intersectable
bool VA::intersect(
    const CollisionPolygonSphere<CompressedScenePos, 3>& t0,
    const IIntersectable& i1,
    IntersectionInfo& intersection_info)
{
    FixedArray<SceneDir, 3> normal = uninitialized;
    ScenePos ray_t = NAN;
    ScenePos overlap;
    if (!i1.touches(t0, overlap, intersection_info.intersection_point, normal)) {
        return false;
    }
    intersection_info.normal0 = t0.polygon.plane.normal;
    intersection_info.ray_t = ray_t;
    intersection_info.no = NormalAndOverlap{ -normal, overlap };
    return true;
}

// Intersectable - ridge
bool VA::intersect(
    const IIntersectable& i0,
    const CollisionRidgeSphere<CompressedScenePos>& r1,
    IntersectionInfo& intersection_info)
{
    FixedArray<ScenePos, 3> intersection_point = uninitialized;
    FixedArray<SceneDir, 3> normal = uninitialized;
    ScenePos overlap;
    if (!i0.touches(r1, overlap, intersection_point, normal)) {
        return false;
    }
    intersection_info.intersection_point = intersection_point;
    intersection_info.normal0 = normal;
    intersection_info.no = NormalAndOverlap{ normal, overlap };
    return true;
}

// Intersectable - line
bool VA::intersect(
    const IIntersectable& i0,
    const CollisionLineSphere<CompressedScenePos>& l1,
    IntersectionInfo& intersection_info)
{
    FixedArray<ScenePos, 3> intersection_point = uninitialized;
    FixedArray<SceneDir, 3> normal = uninitialized;
    ScenePos ray_t;
    ScenePos overlap;
    if (!i0.touches(l1, overlap, ray_t, intersection_point, normal)) {
        return false;
    }
    intersection_info.intersection_point = intersection_point;
    intersection_info.normal0 = normal;
    intersection_info.no = NormalAndOverlap{ normal, overlap };
    return true;
}

// Intersectable - intersectable
bool VA::intersect(
    const IIntersectable& i0,
    const IIntersectable& i1,
    IntersectionInfo& intersection_info)
{
    FixedArray<ScenePos, 3> intersection_point = uninitialized;
    FixedArray<SceneDir, 3> normal = uninitialized;
    ScenePos overlap;
    if (!i0.touches(i1, overlap, intersection_point, normal)) {
        return false;
    }
    intersection_info.intersection_point = intersection_point;
    intersection_info.normal0 = normal;
    intersection_info.no = NormalAndOverlap{ normal, overlap };
    return true;
}

#ifdef __GNUC__
    #pragma GCC pop_options
#endif
