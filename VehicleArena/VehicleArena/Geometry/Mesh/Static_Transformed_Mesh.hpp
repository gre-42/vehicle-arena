// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Bounding_Sphere.hpp>
#include <vector>

namespace VA {

template <class TPos, size_t tndim>
class BoundingSphere;
template <class TDir, class TPos, size_t tndim>
class PlaneNd;
template <class TPosition, size_t tnvertices>
struct CollisionPolygonSphere;
template <class TPosition>
struct CollisionLineSphere;

class StaticTransformedMesh: public IIntersectableMesh {
    StaticTransformedMesh(const StaticTransformedMesh&) = delete;
    StaticTransformedMesh& operator = (const StaticTransformedMesh&) = delete;
public:
    StaticTransformedMesh(
        std::string name,
        const AxisAlignedBoundingBox<CompressedScenePos, 3>& aabb,
        const BoundingSphere<CompressedScenePos, 3>& bounding_sphere,
        std::vector<CollisionPolygonSphere<CompressedScenePos, 4>>&& quads,
        std::vector<CollisionPolygonSphere<CompressedScenePos, 3>>&& triangles,
        std::vector<CollisionLineSphere<CompressedScenePos>>&& lines,
        std::vector<CollisionLineSphere<CompressedScenePos>>&& edges,
        std::vector<TypedMesh<std::shared_ptr<IIntersectable>>>&& intersectables);
    ~StaticTransformedMesh();
    virtual std::string name() const override;
    virtual bool intersects(const BoundingSphere<CompressedScenePos, 3>& sphere) const override;
    virtual bool intersects(const PlaneNd<SceneDir, CompressedScenePos, 3>& plane) const override;
    virtual const std::vector<CollisionPolygonSphere<CompressedScenePos, 4>>& get_quads_sphere() const override;
    virtual const std::vector<CollisionPolygonSphere<CompressedScenePos, 3>>& get_triangles_sphere() const override;
    virtual const std::vector<CollisionLineSphere<CompressedScenePos>>& get_lines_sphere() const override;
    virtual const std::vector<CollisionLineSphere<CompressedScenePos>>& get_edges_sphere() const override;
    virtual const std::vector<TypedMesh<std::shared_ptr<IIntersectable>>>& get_intersectables() const override;
    virtual BoundingSphere<CompressedScenePos, 3> bounding_sphere() const override;
    virtual AxisAlignedBoundingBox<CompressedScenePos, 3> aabb() const override;
private:
    std::string name_;
    AxisAlignedBoundingBox<CompressedScenePos, 3> aabb_;
    BoundingSphere<CompressedScenePos, 3> bounding_sphere_;
    std::vector<CollisionPolygonSphere<CompressedScenePos, 4>> quads_;
    std::vector<CollisionPolygonSphere<CompressedScenePos, 3>> triangles_;
    std::vector<CollisionLineSphere<CompressedScenePos>> lines_;
    std::vector<CollisionLineSphere<CompressedScenePos>> edges_;
    std::vector<TypedMesh<std::shared_ptr<IIntersectable>>> intersectables_;
};

}
