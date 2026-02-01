// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Heli_Controller.hpp"
#include <VehicleArena/Math/Signed_Min.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Rigid_Body/Vehicle_Domain.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Steering_Type.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_State_Updater.hpp>
#include <stdexcept>

using namespace VA;

HeliController::HeliController(
    RigidBodyVehicle& rb,
    std::map<size_t, float> tire_angles,
    size_t main_rotor_id,
    FixedArray<float, 3> angle_multipliers,
    const PidController<double, double>& height_pid,
    VehicleDomain vehicle_domain)
    : RigidBodyVehicleController{ rb, SteeringType::CAR }
    , height_pid_{ height_pid }
    , tire_angles_{ std::move(tire_angles) }
    , main_rotor_id_{ main_rotor_id }
    , angle_multipliers_{ angle_multipliers }
    , vehicle_domain_{ vehicle_domain }
{
    ascend_to(rb.rbp_.abs_position()(1));
}

HeliController::~HeliController() {
    on_destroy.clear();
}

static const size_t PITCH = 0;
static const size_t YAW = 1;
static const size_t ROLL = 2;

static const auto wheels_name = VariableAndHash<std::string>{"wheels"};
static const auto main_rotor_name = VariableAndHash<std::string>{"main_rotor"};
static const auto tail_rotor_name = VariableAndHash<std::string>{"tail_rotor"};

void HeliController::apply() {
    if (vehicle_domain_ == VehicleDomain::AIR) {
        rb_.set_surface_power(wheels_name, EnginePowerIntent{.surface_power = NAN});  // NAN=break
        for (const auto& [x, _] : tire_angles_) {
            rb_.set_tire_angle_y(x, 0.f);
        }
        rb_.set_surface_power(
            main_rotor_name,
            EnginePowerIntent{
                .surface_power = std::isnan(target_height_)
                    ? 0.f
                    : (float)std::min(0., height_pid_(rb_.rbp_.abs_position()(1) - target_height_))});
        rb_.set_rotor_movement_y(main_rotor_id_, std::isnan(surface_power_)
            ? 0.f
            : angle_multipliers_(PITCH) * sign(surface_power_) * drive_relaxation_);
        float ang = signed_min(steer_angle_ * steer_relaxation_, 45.f * degrees);
        rb_.set_rotor_movement_x(main_rotor_id_, angle_multipliers_(ROLL) * ang);
        rb_.set_surface_power(tail_rotor_name, EnginePowerIntent{.surface_power = angle_multipliers_(YAW) * ang});
    } else if (vehicle_domain_ == VehicleDomain::GROUND) {
        rb_.set_surface_power(wheels_name, EnginePowerIntent{
            .surface_power = surface_power_,
            .drive_relaxation = drive_relaxation_});  // NAN=break
        for (const auto& x : tire_angles_) {
            float ang = signed_min(steer_angle_ * steer_relaxation_, x.second);
            rb_.set_tire_angle_y(x.first, ang);
        }
    } else {
        throw std::runtime_error("Unknown vehicle domain");
    }
    if (rb_.animation_state_updater_ != nullptr) {
        rb_.animation_state_updater_->notify_movement_intent();
    }
    RigidBodyVehicleController::apply();
}
