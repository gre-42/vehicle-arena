// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_Triangle_And_Triangles.hpp"
#include <VehicleArena/Geometry/Interfaces/IIntersectable.hpp>
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Ridge.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <VehicleArena/Misc/Pointer_To_Optional.hpp>
#include <VehicleArena/Physics/Collision/Collision_Type.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Collision/Record/Handle_Line_Triangle_Intersection.hpp>
#include <VehicleArena/Physics/Collision/Record/Intersection_Scene.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Surface_Contact_Db.hpp>

using namespace VA;

void VA::collide_triangle_and_triangles(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const IIntersectableMesh* msh0,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const CollisionPolygonSphere<CompressedScenePos, 4>* q0,
    const CollisionPolygonSphere<CompressedScenePos, 3>* t0,
    const CollisionHistory& history)
{
    auto collide = [&](
        const auto& bounding_sphere0,
        const PlaneNd<SceneDir, CompressedScenePos, 3>* plane0,
        PhysicsMaterial physics_material0)
    {
        // Mesh-sphere <-> triangle-sphere intersection
        if (!msh1.mesh->intersects(bounding_sphere0)) {
            return;
        }
        // Mesh-sphere <-> triangle-plane intersection
        if ((plane0 != nullptr) && !msh1.mesh->intersects(*plane0)) {
            return;
        }
        for (const auto& e1 : msh1.mesh->get_edges_sphere()) {
            if (!e1.bounding_sphere.intersects(bounding_sphere0)) {
                continue;
            }
            handle_line_triangle_intersection(IntersectionScene{
                .o0 = o0,
                .o1 = o1,
                .mesh0 = msh0,
                .mesh1 = msh1.mesh.get(),
                .l1 = e1,
                .r1 = std::nullopt,
                .q0 = pointer_to_optional(q0),
                .t0 = pointer_to_optional(t0),
                .tire_id1 = SIZE_MAX,
                .mesh0_material = physics_material0,
                .mesh1_material = msh1.physics_material,
                .l1_is_normal = false,
                .surface_contact_info = history.surface_contact_db.get_contact_info(
                    physics_material0,
                    msh1.physics_material),
                .default_collision_type = CollisionType::REFLECT,
                .history = history});
        }
    };
    if (q0 != nullptr) {
        collide(q0->bounding_sphere, &q0->polygon.plane, q0->physics_material);
    }
    if (t0 != nullptr) {
        collide(t0->bounding_sphere, &t0->polygon.plane, t0->physics_material);
    }
}

void VA::collide_triangle_and_triangles(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const IIntersectableMesh* msh0,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const std::variant<CollisionPolygonSphere<CompressedScenePos, 3>, CollisionPolygonSphere<CompressedScenePos, 4>>& cps0,
    const CollisionHistory& history)
{
    collide_triangle_and_triangles(
        o0,
        o1,
        msh0,
        msh1,
        std::get_if<CollisionPolygonSphere<CompressedScenePos, 4>>(&cps0),
        std::get_if<CollisionPolygonSphere<CompressedScenePos, 3>>(&cps0),
        history);
}
