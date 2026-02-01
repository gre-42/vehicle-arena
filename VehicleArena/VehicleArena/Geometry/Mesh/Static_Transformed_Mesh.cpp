// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Static_Transformed_Mesh.hpp"
#include <VehicleArena/Geometry/Interfaces/IIntersectable.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Line.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Geometry/Primitives/Plane_Nd.hpp>

using namespace VA;

#ifdef __GNUC__
    #pragma GCC push_options
    #pragma GCC optimize ("O3")
#endif

StaticTransformedMesh::StaticTransformedMesh(
    std::string name,
    const AxisAlignedBoundingBox<CompressedScenePos, 3>& aabb,
    const BoundingSphere<CompressedScenePos, 3>& bounding_sphere,
    std::vector<CollisionPolygonSphere<CompressedScenePos, 4>>&& quads,
    std::vector<CollisionPolygonSphere<CompressedScenePos, 3>>&& triangles,
    std::vector<CollisionLineSphere<CompressedScenePos>>&& lines,
    std::vector<CollisionLineSphere<CompressedScenePos>>&& edges,
    std::vector<TypedMesh<std::shared_ptr<IIntersectable>>>&& intersectables)
    : name_{ std::move(name) }
    , aabb_{ aabb }
    , bounding_sphere_{ bounding_sphere }
    , quads_{ std::move(quads) }
    , triangles_{ std::move(triangles) }
    , lines_{ std::move(lines) }
    , edges_{ std::move(edges) }
    , intersectables_{ std::move(intersectables) }
{}

StaticTransformedMesh::~StaticTransformedMesh() = default;

bool StaticTransformedMesh::intersects(const BoundingSphere<CompressedScenePos, 3>& sphere) const {
    return bounding_sphere_.intersects(sphere);
}

bool StaticTransformedMesh::intersects(const PlaneNd<SceneDir, CompressedScenePos, 3>& plane) const {
    return bounding_sphere_.intersects(plane);
}

const std::vector<CollisionPolygonSphere<CompressedScenePos, 4>>& StaticTransformedMesh::get_quads_sphere() const {
    return quads_;
}

const std::vector<CollisionPolygonSphere<CompressedScenePos, 3>>& StaticTransformedMesh::get_triangles_sphere() const {
    return triangles_;
}

const std::vector<CollisionLineSphere<CompressedScenePos>>& StaticTransformedMesh::get_lines_sphere() const {
    return lines_;
}

const std::vector<CollisionLineSphere<CompressedScenePos>>& StaticTransformedMesh::get_edges_sphere() const {
    return edges_;
}

const std::vector<TypedMesh<std::shared_ptr<IIntersectable>>>& StaticTransformedMesh::get_intersectables() const
{
    return intersectables_;
}

BoundingSphere<CompressedScenePos, 3> StaticTransformedMesh::bounding_sphere() const {
    return bounding_sphere_;
}

AxisAlignedBoundingBox<CompressedScenePos, 3> StaticTransformedMesh::aabb() const {
    return aabb_;
}

std::string StaticTransformedMesh::name() const {
    return name_;
}

#ifdef __GNUC__
    #pragma GCC pop_options
#endif
