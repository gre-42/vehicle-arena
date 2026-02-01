// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_Triangles_And_Line.hpp"
#include <VehicleArena/Geometry/Interfaces/IIntersectable.hpp>
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Line.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Misc/Pointer_To_Optional.hpp>
#include <VehicleArena/Physics/Collision/Collision_Type.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Collision/Record/Handle_Line_Triangle_Intersection.hpp>
#include <VehicleArena/Physics/Collision/Record/Intersection_Scene.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Surface_Contact_Db.hpp>

using namespace VA;

void VA::collide_triangles_and_line(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh0,
    const CollisionLineSphere<CompressedScenePos>& e1,
    const CollisionHistory& history)
{
    auto non_tire_line_mask =
        PhysicsMaterial::OBJ_CHASSIS |
        PhysicsMaterial::OBJ_BULLET_LINE_SEGMENT |
        PhysicsMaterial::OBJ_ALIGNMENT_CONTACT |
        PhysicsMaterial::OBJ_DISTANCEBOX;
    if (!e1.bounding_sphere.intersects(msh0.mesh->bounding_sphere())) {
        return;
    }
    auto collide = [&](
        PhysicsMaterial physics_material0,
        const BoundingSphere<CompressedScenePos, 3>& bounding_sphere0,
        const CollisionPolygonSphere<CompressedScenePos, 4>* q0,
        const CollisionPolygonSphere<CompressedScenePos, 3>* t0,
        const IIntersectable* i0)
    {
        if (!any(physics_material0 & non_tire_line_mask)) {
            return;
        }
        if (!e1.bounding_sphere.intersects(bounding_sphere0)) {
            return;
        }
        handle_line_triangle_intersection(IntersectionScene{
            .o0 = o0,
            .o1 = o1,
            .mesh0 = msh0.mesh.get(),
            .mesh1 = nullptr,
            .l1 = e1,
            .r1 = std::nullopt,
            .q0 = pointer_to_optional(q0),
            .t0 = pointer_to_optional(t0),
            .i0 = i0,
            .tire_id1 = SIZE_MAX,
            .mesh0_material = physics_material0,
            .mesh1_material = e1.physics_material,
            .l1_is_normal = false,
            .surface_contact_info = history.surface_contact_db.get_contact_info(
                physics_material0,
                e1.physics_material),
            .default_collision_type = CollisionType::REFLECT,
            .history = history});
    };
    for (const auto& q0 : msh0.mesh->get_quads_sphere()) {
        collide(q0.physics_material, q0.bounding_sphere, &q0, nullptr, nullptr);
    }
    for (const auto& t0 : msh0.mesh->get_triangles_sphere()) {
        collide(t0.physics_material, t0.bounding_sphere, nullptr, &t0, nullptr);
    }
    for (const auto& i0 : msh0.mesh->get_intersectables()) {
        collide(i0.physics_material, i0.mesh->bounding_sphere(), nullptr, nullptr, i0.mesh.get());
    }
}
