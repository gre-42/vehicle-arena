// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_Line_And_Triangles.hpp"
#include <VehicleArena/Geometry/Interfaces/IIntersectable.hpp>
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Line.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Physics/Collision/Collision_Type.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Collision/Record/Handle_Line_Triangle_Intersection.hpp>
#include <VehicleArena/Physics/Collision/Record/Intersection_Scene.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Surface_Contact_Db.hpp>

using namespace VA;

void VA::collide_line_and_triangles(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const IIntersectableMesh& msh1,
    const CollisionLineSphere<CompressedScenePos>& l0,
    const CollisionHistory& history)
{
    // Mesh-sphere <-> line-sphere intersection
    if (!msh1.intersects(l0.bounding_sphere)) {
        return;
    }
    for (const auto& i1 : msh1.get_intersectables()) {
        if (!i1.mesh->bounding_sphere().intersects(l0.bounding_sphere)) {
            continue;
        }
        handle_line_triangle_intersection(IntersectionScene{
            .o0 = o1,
            .o1 = o0,
            .mesh0 = nullptr, // msh1.mesh,
            .mesh1 = nullptr, // msh0,
            .l1 = l0,
            .r1 = std::nullopt,
            .q0 = std::nullopt,
            .t0 = std::nullopt,
            .i0 = i1.mesh.get(),
            .tire_id1 = SIZE_MAX,
            .mesh0_material = i1.physics_material,
            .mesh1_material = l0.physics_material,
            .l1_is_normal = false,
            .surface_contact_info = history.surface_contact_db.get_contact_info(
                l0.physics_material,
                i1.physics_material),
            .default_collision_type = CollisionType::GRIND,
            .history = history});
    }
}
