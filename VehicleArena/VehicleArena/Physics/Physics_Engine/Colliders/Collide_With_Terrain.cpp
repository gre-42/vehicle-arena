// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_With_Terrain.hpp"
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Line_And_Triangles.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangle_And_Edges.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangle_And_Intersectables.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangle_And_Lines.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangle_And_Triangles.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangles_And_Line.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Containers/Collision_Group.hpp>
#include <VehicleArena/Physics/Containers/Rigid_Bodies.hpp>
#include <VehicleArena/Physics/Physics_Engine/Colliders/Collide_Convex_Meshes.hpp>
#include <VehicleArena/Physics/Physics_Engine/Physics_Phase.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <stdexcept>

namespace VA {
    
inline bool intersects(
    const AxisAlignedBoundingBox<ScenePos, 3>& a,
    const AxisAlignedBoundingBox<CompressedScenePos, 3>& b)
{
    return a.intersects(b.casted<ScenePos>());
}

}

using namespace VA;

void VA::collide_with_terrain(
    RigidBodies& rigid_bodies,
    const CollisionHistory& history)
{
    for (const auto& o1 : rigid_bodies.transformed_objects()) {
        if (o1.rigid_body->mass() == INFINITY) {
            continue;
        }
        for (const auto& msh1 : o1.meshes) {
            if (!history.phase.group.rigid_bodies.contains(&o1.rigid_body->rbp_)) {
                continue;
            }
            PhysicsMaterial collide_with_terrain_triangle_mask =
                PhysicsMaterial::OBJ_CHASSIS |
                PhysicsMaterial::OBJ_TIRE_LINE |
                PhysicsMaterial::OBJ_BULLET_MASK |
                PhysicsMaterial::OBJ_ALIGNMENT_CONTACT |
                PhysicsMaterial::OBJ_DISTANCEBOX;
            if (any(msh1.physics_material & collide_with_terrain_triangle_mask)) {
                if (any(msh1.physics_material & PhysicsMaterial::ATTR_CONVEX) ||
                    any(msh1.physics_material & PhysicsMaterial::OBJ_TIRE_LINE) ||
                    any(msh1.physics_material & PhysicsMaterial::OBJ_BULLET_MASK))
                {
                    rigid_bodies.convex_mesh_bvh().grid().visit(
                        msh1.mesh->aabb(),
                        [&](const RigidBodyAndIntersectableMesh& rm) {
                            collide_convex_meshes(
                                rm.rb.get(),
                                o1.rigid_body.get(),
                                rm.mesh,
                                msh1,
                                history);
                            return true;
                        });
                }
                rigid_bodies.triangle_bvh().grid().visit(
                    msh1.mesh->aabb(),
                    [&](const RigidBodyAndCollisionTriangleSphere<CompressedScenePos>& t0){
                        return std::visit([&](const auto& ctp)
                            {
                                if (any(ctp.physics_material & PhysicsMaterial::ATTR_CONVEX) &&
                                    any(msh1.physics_material & PhysicsMaterial::ATTR_CONVEX))
                                {
                                    return true;
                                }
                                if (any(msh1.physics_material & PhysicsMaterial::OBJ_BULLET_MESH) &&
                                    !any(msh1.physics_material & PhysicsMaterial::ATTR_CONVEX))
                                {
                                    collide_triangle_and_triangles(
                                        t0.rb,
                                        o1.rigid_body.get(),
                                        nullptr,
                                        msh1,
                                        ctp,
                                        history);
                                }
                                collide_triangle_and_edges(
                                    t0.rb,
                                    o1.rigid_body.get(),
                                    msh1,
                                    ctp,
                                    history);
                                collide_triangle_and_lines(
                                    t0.rb,
                                    o1.rigid_body.get(),
                                    msh1,
                                    ctp,
                                    history);
                                collide_triangle_and_intersectables(
                                    t0.rb,
                                    o1.rigid_body.get(),
                                    msh1,
                                    ctp,
                                    history);
                                return true;
                            },
                            t0.ctp);
                    });
                rigid_bodies.line_bvh().visit(
                    msh1.mesh->aabb(),
                    [&](const RigidBodyAndCollisionLineSphere<CompressedScenePos>& e0){
                        collide_triangles_and_line(
                            o1.rigid_body.get(),
                            e0.rb,
                            msh1,
                            e0.clp,
                            history);
                        return true;
                    });
            } else if (any(msh1.physics_material & PhysicsMaterial::OBJ_GRIND_CONTACT)) {
                rigid_bodies.line_bvh().visit(
                    msh1.mesh->aabb(),
                    [&](const RigidBodyAndCollisionLineSphere<CompressedScenePos>& l0){
                        collide_line_and_triangles(
                            l0.rb,
                            o1.rigid_body.get(),
                            *msh1.mesh,
                            l0.clp,
                            history);
                        return true;
                    });
            } else if (any(msh1.physics_material & PhysicsMaterial::OBJ_HITBOX)) {
                if (!msh1.mesh->get_lines_sphere().empty()) {
                    throw std::runtime_error("Detected hitbox with lines in object \"" + o1.rigid_body->name() + '"');
                }
            } else {
                throw std::runtime_error(
                    "Unknown mesh type when colliding object \"" + o1.rigid_body->name() + '"');
            }
        }
    }
}
