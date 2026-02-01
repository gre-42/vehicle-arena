// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Threads/Safe_Atomic_Shared_Mutex.hpp>
#include <cstddef>
#include <string>
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
template <class TData, size_t tndim>
class AxisAlignedBoundingBox;
class CollisionVertices;
class IIntersectable;
template <class T>
struct TypedMesh;

class IIntersectableMesh {
public:
    IIntersectableMesh();
    virtual ~IIntersectableMesh();
    virtual std::string name() const = 0;
    bool intersects(const IIntersectableMesh& other) const;
    virtual bool intersects(const BoundingSphere<CompressedScenePos, 3>& sphere) const = 0;
    virtual bool intersects(const PlaneNd<SceneDir, CompressedScenePos, 3>& plane) const = 0;
    template <size_t tnvertices>
    inline const std::vector<CollisionPolygonSphere<CompressedScenePos, tnvertices>>& get_polygons_sphere() const {
        if constexpr (tnvertices == 4) {
            return get_quads_sphere();
        } else if constexpr (tnvertices == 3) {
            return get_triangles_sphere();
        } else {
            // From: https://stackoverflow.com/questions/38304847/constexpr-if-and-static-assert
            static_assert(tnvertices == 4, "Unknown vertex-count");
        }
    }
    virtual const std::vector<CollisionPolygonSphere<CompressedScenePos, 4>>& get_quads_sphere() const = 0;
    virtual const std::vector<CollisionPolygonSphere<CompressedScenePos, 3>>& get_triangles_sphere() const = 0;
    virtual const std::vector<CollisionLineSphere<CompressedScenePos>>& get_lines_sphere() const = 0;
    virtual const std::vector<CollisionLineSphere<CompressedScenePos>>& get_edges_sphere() const = 0;
    virtual const std::vector<TypedMesh<std::shared_ptr<IIntersectable>>>& get_intersectables() const = 0;
    virtual BoundingSphere<CompressedScenePos, 3> bounding_sphere() const = 0;
    virtual AxisAlignedBoundingBox<CompressedScenePos, 3> aabb() const = 0;
};

}
