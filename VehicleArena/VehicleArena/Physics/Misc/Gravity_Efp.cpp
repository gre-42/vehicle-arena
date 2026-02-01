// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Gravity_Efp.hpp"
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Fixed_Scaled_Unit_Vector.hpp>
#include <VehicleArena/Physics/Physics_Engine/Physics_Engine.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle_Flags.hpp>
#include <VehicleArena/Scene_Config/Physics_Engine_Config.hpp>
#include <VehicleArena/Scene_Graph/Instances/Static_World.hpp>

using namespace VA;

GravityEfp::GravityEfp(PhysicsEngine& engine)
    : engine_{ engine }
{}

GravityEfp::~GravityEfp() = default;

void GravityEfp::increment_external_forces(
    const PhysicsEngineConfig& cfg,
    const PhysicsPhase& phase,
    const StaticWorld& world)
{
    if (world.gravity == nullptr) {
        return;
    }
    for (auto& rb : engine_.rigid_bodies_.objects()) {
        if (rb.rigid_body->feels_gravity() &&
            (rb.rigid_body->mass() != INFINITY) &&
            phase.group.rigid_bodies.contains(&rb.rigid_body->rbp_))
        {
            if (rb.rigid_body->is_deactivated_avatar()) {
                throw std::runtime_error("Attempt to integrate gravity of deactivated avatar");
            }
            auto dt = cfg.dt_substeps(phase);
            rb.rigid_body->rbp_.integrate_delta_v(world.gravity->vector * dt, dt);
        }
    }
}
