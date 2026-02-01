// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Missile_Controller.hpp"
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <stdexcept>

using namespace VA;

MissileController::MissileController(
    RigidBodyVehicle& rb,
    float dt_ref,
    const PidController<FixedArray<float, 2>, float>& pid,
    std::vector<MissileWingController> wing_controllers,
    VariableAndHash<std::string> engine_name)
    : RigidBodyMissileController{ rb }
    , dt_ref_{ dt_ref }
    , pid_{ pid }
    , wing_controllers_{ std::move(wing_controllers) }
    , engine_name_{ std::move(engine_name) }
{}

MissileController::~MissileController() = default;

void MissileController::apply(float dt) {
    rb_.set_surface_power(engine_name_, EnginePowerIntent{
        .surface_power = rocket_engine_power_,
        .drive_relaxation = rocket_engine_power_relaxation_ });

    auto rel_dir = dot(desired_direction_, rb_.rbp_.rotation_);
    FixedArray<float, 2> fake_dir{ rel_dir(0), rel_dir(1) };
    fake_dir = pid_(fake_dir, dt_ref_, dt);
    // Check if the target is behind the missile
    if (rel_dir(2) > -1e-12) {
        float l2 = std::sqrt(sum(squared(fake_dir)));
        if (l2 < 1e-12) {
            fake_dir = { 1.f, 0.f };
        } else {
            fake_dir /= l2;
        }
    }
    auto roll_strength = rb_.rbp_.rotation_(1, 0);
    for (const auto& wing_controller : wing_controllers_) {
        auto angle = dot0d(wing_controller.gain, fake_dir) + wing_controller.antiroll_angle * roll_strength;
        auto max_error = 4 * M_PI;
        if (std::abs(angle) > max_error) {
            throw std::runtime_error(
                "Missile wing angle too large. Actual: " +
                std::to_string(angle) + ". Max (error): " +
                std::to_string(max_error));
        }
        rb_.set_wing_angle_of_attack(
            wing_controller.i,
                std::clamp(
                    angle,
                    -wing_controller.max_angle,
                    wing_controller.max_angle));
    }
}
