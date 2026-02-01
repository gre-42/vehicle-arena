// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Primitives/Bvh.hpp>
#include <VehicleArena/Geometry/Primitives/Bvh_Grid.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Line.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Ridge.hpp>
#include <VehicleArena/Iterator/Iterable_Wrapper.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Physics/Containers/Elements/Collision_Line_Sphere.hpp>
#include <VehicleArena/Physics/Containers/Elements/Collision_Ridge_Sphere.hpp>
#include <VehicleArena/Physics/Containers/Elements/Collision_Triangle_Sphere.hpp>
#include <VehicleArena/Physics/Containers/Ridge_Map.hpp>
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <variant>

namespace VA {

enum class CollidableMode;
template <class TPos>
class ColoredVertexArray;
class RigidBodyVehicle;
class IIntersectableMesh;
struct PhysicsEngineConfig;
template <class T>
class DestructionFunctionsTokensRef;
class CollisionMesh;
class IIntersectable;
struct CollisionGroup;

struct RigidBodyAndMeshes {
    using Mesh = TypedMesh<std::pair<BoundingSphere<CompressedScenePos, 3>, std::shared_ptr<CollisionMesh>>>;
    DanglingBaseClassRef<RigidBodyVehicle> rigid_body;
    std::list<Mesh> meshes;
    inline bool has_meshes() const {
        return !meshes.empty();
    }
};

struct RigidBodyAndIntersectableMeshes {
    DanglingBaseClassRef<RigidBodyVehicle> rigid_body;
    std::list<TypedMesh<std::shared_ptr<IIntersectableMesh>>> meshes;
};

struct RigidBodyAndIntersectableMesh {
    DanglingBaseClassRef<RigidBodyVehicle> rb;
    TypedMesh<std::shared_ptr<IIntersectableMesh>> mesh;
};

enum class CollisionRidgeBakingStatus {
    NOT_BAKED,
    BAKED,
    BAKING
};

class RigidBodies {
    friend class PhysicsEngine;
public:
    using ConvexMeshBvh = BvhGrid<
        CompressedScenePos,
        3,
        RigidBodyAndIntersectableMesh>;
    using TriangleBvh = CompressedBvhGrid<
        CompressedScenePos,
        HalfCompressedScenePos,
        RigidBodyAndCollisionTriangleSphere<CompressedScenePos>,
        RigidBodyAndCollisionTriangleSphere<HalfCompressedScenePos>,
        3>;
    using RidgeBvh = CompressedBvh<
        CompressedScenePos,
        HalfCompressedScenePos,
        RigidBodyAndCollisionRidgeSphere<CompressedScenePos>,
        RigidBodyAndCollisionRidgeSphere<HalfCompressedScenePos>,
        3>;
    using LineBvh = CompressedBvh<
        CompressedScenePos,
        HalfCompressedScenePos,
        RigidBodyAndCollisionLineSphere<CompressedScenePos>,
        RigidBodyAndCollisionLineSphere<HalfCompressedScenePos>,
        3>;

    explicit RigidBodies(const PhysicsEngineConfig& cfg);
    ~RigidBodies();
    void add_rigid_body(
        RigidBodyVehicle& rigid_body,
        const std::list<std::shared_ptr<ColoredVertexArray<float>>>& s_hitboxes,
        const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& d_hitboxes,
        const std::list<TypedMesh<std::shared_ptr<IIntersectable>>>& intersectables,
        CollidableMode collidable_mode);
    void delete_rigid_body(const RigidBodyVehicle& rigid_body);
    void optimize_search_time(std::ostream& ostr) const;
    void print_search_time() const;
    void print_compression_ratio() const;
    void plot_convex_mesh_bvh_svg(const std::string& filename, size_t axis0, size_t axis1) const;
    void plot_triangle_bvh_svg(const std::string& filename, size_t axis0, size_t axis1) const;
    void plot_line_bvh_svg(const std::string& filename, size_t axis0, size_t axis1) const;
    IterableWrapper<std::list<RigidBodyAndMeshes>> objects() const;
    IterableWrapper<std::list<RigidBodyAndIntersectableMeshes>> transformed_objects() const;
    const ConvexMeshBvh& convex_mesh_bvh() const;
    const TriangleBvh& triangle_bvh() const;
    const LineBvh& line_bvh() const;
    bool empty() const;
    std::vector<CollisionGroup> collision_groups();
    void notify_colliding_start();
    void notify_colliding_end();
private:
    void transform_object_and_add(const RigidBodyAndMeshes& o);
    const PhysicsEngineConfig& cfg_;
    std::unordered_map<const RigidBodyVehicle*, DestructionFunctionsTokensRef<RigidBodyVehicle>> rigid_bodies_;
    std::list<RigidBodyAndMeshes> objects_;
    std::list<RigidBodyAndIntersectableMeshes> transformed_objects_;
    bool is_colliding_;
    std::map<const RigidBodyVehicle*, CollidableMode> collidable_modes_;
    // BVHs. Do not forget to .clear() the BVHs in the "delete_rigid_body" method.
    ConvexMeshBvh convex_mesh_bvh_;
    TriangleBvh triangle_bvh_;
    LineBvh line_bvh_;
};

}
