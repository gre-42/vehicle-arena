// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array_Forward.hpp>
#include <VehicleArena/Physics/Collision/Collision_Type.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstdint>
#include <list>
#include <memory>

namespace VA {

enum class PhysicsMaterial: uint32_t;
class RigidBodyVehicle;
class BaseLog;
struct StaticWorld;

enum class CollisionRole {
    PRIMARY,
    SECONDARY
};

class CollisionObserver {
public:
    // Called by rigid body's destructor
    virtual ~CollisionObserver() = default;
    virtual void notify_collided(
        const FixedArray<ScenePos, 3>& intersection_point,
        const StaticWorld& world,
        RigidBodyVehicle& rigid_body,
        PhysicsMaterial physics_material,
        CollisionRole collision_role,
        CollisionType& collision_type,
        bool& abort) {};
    virtual void notify_impact(
        RigidBodyVehicle& rigid_body,
        PhysicsMaterial physics_material,
        CollisionRole collision_role,
        const FixedArray<float, 3>& normal,
        float lambda_final,
        BaseLog* base_log) {};
};

}
