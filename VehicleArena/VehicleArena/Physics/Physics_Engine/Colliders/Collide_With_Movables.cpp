// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_With_Movables.hpp"
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Iterator/Enumerate.hpp>
#include <VehicleArena/Iterator/Reverse_Iterator.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Containers/Collision_Group.hpp>
#include <VehicleArena/Physics/Containers/Rigid_Bodies.hpp>
#include <VehicleArena/Physics/Physics_Engine/Colliders/Collide_Convex_Meshes.hpp>
#include <VehicleArena/Physics/Physics_Engine/Physics_Phase.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <stdexcept>

using namespace VA;

static void collide_objects(
    const RigidBodyAndIntersectableMeshes& o0,
    const RigidBodyAndIntersectableMeshes& o1,
    const CollisionHistory& history)
{
    if (&o0.rigid_body == &o1.rigid_body) {
        throw std::runtime_error("Cannot collide identical objects");
    }
    if ((o0.rigid_body->mass() == INFINITY) && (o1.rigid_body->mass() == INFINITY)) {
        return;
    }
    if (!history.phase.group.rigid_bodies.contains(&o0.rigid_body->rbp_) &&
        !history.phase.group.rigid_bodies.contains(&o1.rigid_body->rbp_))
    {
        return;
    }
    if (o0.rigid_body->non_colliders_.contains(o1.rigid_body) ||
        o1.rigid_body->non_colliders_.contains(o0.rigid_body))
    {
        return;
    }
    PhysicsMaterial included_materials =
        PhysicsMaterial::OBJ_BULLET_COLLIDABLE_MASK |
        PhysicsMaterial::OBJ_BULLET_MASK |
        PhysicsMaterial::OBJ_DISTANCEBOX;
    for (const auto& msh1 : o1.meshes) {
        if (!any(msh1.physics_material & included_materials)) {
            continue;
        }
        for (const auto& msh0 : o0.meshes) {
            if (!any(msh0.physics_material & included_materials)) {
                continue;
            }
            collide_convex_meshes(
                o0.rigid_body.get(),
                o1.rigid_body.get(),
                msh0,
                msh1,
                history);
        }
    }
}

void VA::collide_with_movables(
    CollisionDirection collision_direction,
    RigidBodies& rigid_bodies,
    const CollisionHistory& history)
{
    if (collision_direction == CollisionDirection::FORWARD) {
        for (const auto& [i0, o0] : enumerate(rigid_bodies.transformed_objects())) {
            for (const auto& [i1, o1] : enumerate(rigid_bodies.transformed_objects())) {
                if (i1 >= i0) {
                    break;
                }
                collide_objects(o0, o1, history);
            }
        }
    } else {
        for (const auto& [i0, o0] : enumerate(reverse(rigid_bodies.transformed_objects()))) {
            for (const auto& [i1, o1] : enumerate(reverse(rigid_bodies.transformed_objects()))) {
                if (i1 >= i0) {
                    break;
                }
                collide_objects(o0, o1, history);
            }
        }
    }
}
