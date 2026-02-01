// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_Triangle_And_Edges.hpp"
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Ridge.hpp>
#include <VehicleArena/Misc/Pointer_To_Optional.hpp>
#include <VehicleArena/Physics/Collision/Collision_Type.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Collision/Record/Handle_Line_Triangle_Intersection.hpp>
#include <VehicleArena/Physics/Collision/Record/Intersection_Scene.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Surface_Contact_Db.hpp>

using namespace VA;

void VA::collide_triangle_and_edges(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const std::variant<CollisionPolygonSphere<CompressedScenePos, 3>, CollisionPolygonSphere<CompressedScenePos, 4>>& vcps0,
    const CollisionHistory& history)
{
    auto non_tire_line_mask =
        PhysicsMaterial::OBJ_CHASSIS |
        PhysicsMaterial::OBJ_BULLET_LINE_SEGMENT |
        PhysicsMaterial::OBJ_DISTANCEBOX;
    std::visit([&](const auto& cps0) {
        if (any(msh1.physics_material & non_tire_line_mask)) {
            for (const auto& e1 : msh1.mesh->get_edges_sphere()) {
                if (!e1.bounding_sphere.intersects(cps0.bounding_sphere)) {
                    continue;
                }
                if (!e1.bounding_sphere.intersects(cps0.polygon.plane)) {
                    continue;
                }
                handle_line_triangle_intersection(IntersectionScene{
                    .o0 = o0,
                    .o1 = o1,
                    .mesh0 = nullptr,
                    .mesh1 = msh1.mesh.get(),
                    .l1 = e1,
                    .r1 = std::nullopt,
                    .q0 = pointer_to_optional(std::get_if<CollisionPolygonSphere<CompressedScenePos, 4>>(&vcps0)),
                    .t0 = pointer_to_optional(std::get_if<CollisionPolygonSphere<CompressedScenePos, 3>>(&vcps0)),
                    .tire_id1 = SIZE_MAX,
                    .mesh0_material = cps0.physics_material,
                    .mesh1_material = msh1.physics_material,
                    .l1_is_normal = false,
                    .surface_contact_info = history.surface_contact_db.get_contact_info(
                        cps0.physics_material,
                        msh1.physics_material),
                    .default_collision_type = CollisionType::REFLECT,
                    .history = history});
            }
        }
    },
    vcps0);
}
