// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_Convex_Meshes.hpp"
#include <VehicleArena/Geometry/Mesh/IIntersectable_Mesh.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Geometry/Primitives/Collision_Polygon.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Intersectables_And_Intersectables.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangle_And_Intersectables.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangle_And_Lines.hpp>
#include <VehicleArena/Physics/Collision/Detect/Collide_Triangle_And_Triangles.hpp>
#include <VehicleArena/Physics/Collision/Record/Collision_History.hpp>
#include <VehicleArena/Physics/Containers/Collision_Group.hpp>
#include <VehicleArena/Physics/Physics_Engine/Physics_Phase.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

using namespace VA;

static void collide(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh0,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const CollisionHistory& history,
    PhysicsMaterial line_mask,
    const CollisionPolygonSphere<CompressedScenePos, 4>* q0,
    const CollisionPolygonSphere<CompressedScenePos, 3>* t0)
{
    collide_triangle_and_triangles(
        o0,
        o1,
        msh0.mesh.get(),
        msh1,
        q0,
        t0,
        history);
    collide_triangle_and_intersectables(
        o0,
        o1,
        msh1,
        q0,
        t0,
        history);
    if (any(msh1.physics_material & line_mask)) {
        collide_triangle_and_lines(
            o0,
            o1,
            msh1,
            q0,
            t0,
            history);
    }
}

static void collide(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh0,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const CollisionHistory& history,
    PhysicsMaterial line_mask)
{
    for (const auto& q0 : msh0.mesh->get_quads_sphere()) {
        collide(o0, o1, msh0, msh1, history, line_mask, &q0, nullptr);
    }
    for (const auto& t0 : msh0.mesh->get_triangles_sphere()) {
        collide(o0, o1, msh0, msh1, history, line_mask, nullptr, &t0);
    }
}

void VA::collide_convex_meshes(
    RigidBodyVehicle& o0,
    RigidBodyVehicle& o1,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh0,
    const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh1,
    const CollisionHistory& history)
{
    auto check_material = [](const RigidBodyVehicle& o, const TypedMesh<std::shared_ptr<IIntersectableMesh>>& msh){
        PhysicsMaterial known_objects =
            PhysicsMaterial::OBJ_CHASSIS |
            PhysicsMaterial::OBJ_TIRE_LINE |
            PhysicsMaterial::OBJ_BULLET_LINE_SEGMENT |
            PhysicsMaterial::OBJ_BULLET_MESH |
            PhysicsMaterial::OBJ_HITBOX |
            PhysicsMaterial::OBJ_DISTANCEBOX;

        if (!any(msh.physics_material & known_objects)) {
            throw std::runtime_error(
                "Unexpected material for convex mesh \"" + msh.mesh->name() +
                "\" in object \"" + o.name() + "\". Expected: " +
                physics_material_to_string(known_objects) + ". Actual: " +
                physics_material_to_string(msh.physics_material));
        }
    };
    check_material(o0, msh0);
    check_material(o1, msh1);

    PhysicsMaterial combined_material = (msh0.physics_material | msh1.physics_material);
    if (any(combined_material & PhysicsMaterial::OBJ_BULLET_MASK) &&
       !any(combined_material & PhysicsMaterial::OBJ_BULLET_COLLIDABLE_MASK))
    {
        return;
    }
    if (!any(combined_material & PhysicsMaterial::OBJ_BULLET_MASK) &&
         any(combined_material & PhysicsMaterial::OBJ_HITBOX))
    {
        return;
    }
    if (!msh0.mesh->intersects(*msh1.mesh)) {
        return;
    }
    auto line_mask = [&](){
        switch (history.phase.group.penetration_class) {
            case PenetrationClass::NONE:
                throw std::runtime_error("PenetrationClass::NONE in collide_convex_meshes");
            case PenetrationClass::BULLET_LINE:
                return PhysicsMaterial::OBJ_BULLET_LINE_SEGMENT;
            case PenetrationClass::STANDARD:
                return PhysicsMaterial::OBJ_TIRE_LINE;
        }
        throw std::runtime_error("Unknown penetration class");
    }();
    collide(o0, o1, msh0, msh1, history, line_mask);
    collide(o1, o0, msh1, msh0, history, line_mask);
    collide_intersectables_and_intersectables(
        o0,
        o1,
        msh0,
        msh1,
        history);
}
