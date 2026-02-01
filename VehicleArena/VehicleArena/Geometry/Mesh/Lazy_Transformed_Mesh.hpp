// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array_Forward.hpp>
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Primitives/Bounding_Sphere.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <atomic>
#include <cstdint>
#include <vector>

namespace VA {

class CollisionMesh;
enum class PhysicsMaterial: uint32_t;
template <class TPosition, size_t tnvertices>
struct CollisionPolygonSphere;
template <class TPosition>
struct CollisionLineSphere;
template <class T>
struct TypedMesh;

class LazyTransformedMesh: public IIntersectableMesh {
    LazyTransformedMesh(const LazyTransformedMesh&) = delete;
    LazyTransformedMesh& operator = (const LazyTransformedMesh&) = delete;
public:
    LazyTransformedMesh(
        const TransformationMatrix<SceneDir, ScenePos, 3>& transformation_matrix,
        const BoundingSphere<CompressedScenePos, 3>& bounding_sphere,
        const std::shared_ptr<CollisionMesh>& collision_mesh);
    ~LazyTransformedMesh();
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
    void print_info() const;
private:
    const TransformationMatrix<float, ScenePos, 3> transformation_matrix_;
    BoundingSphere<CompressedScenePos, 3> transformed_bounding_sphere_;
    std::shared_ptr<CollisionMesh> mesh_;
    mutable std::vector<TypedMesh<std::shared_ptr<IIntersectable>>> intersectables_;
    mutable std::vector<CollisionPolygonSphere<CompressedScenePos, 4>> transformed_quads_;
    mutable std::vector<CollisionPolygonSphere<CompressedScenePos, 3>> transformed_triangles_;
    mutable std::vector<CollisionLineSphere<CompressedScenePos>> transformed_lines_;
    mutable std::vector<CollisionLineSphere<CompressedScenePos>> transformed_edges_;
    mutable std::vector<TypedMesh<std::shared_ptr<IIntersectable>>> transformed_intersectables_;
    mutable FastMutex mutex_;
    mutable std::atomic_bool quads_calculated_ = false;
    mutable std::atomic_bool triangles_calculated_ = false;
    mutable std::atomic_bool lines_calculated_ = false;
    mutable std::atomic_bool edges_calculated_ = false;
    mutable std::atomic_bool intersectables_calculated_ = false;
};

}
