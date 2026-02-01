// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Tank_Controller.hpp"
#include <VehicleArena/Physics/Actuators/Engine_Power_Delta_Intent.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Steering_Type.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_State_Updater.hpp>
#include <stdexcept>

using namespace VA;

TankController::TankController(
    RigidBodyVehicle& rb,
    const std::vector<size_t>& left_tires,
    const std::vector<size_t>& right_tires,
    float steering_multiplier)
: RigidBodyVehicleController{ rb, SteeringType::TANK },
  left_tires_{ left_tires },
  right_tires_{ right_tires },
  delta_power_{ steering_multiplier }
{}

TankController::~TankController() {
    on_destroy.clear();
}

static const auto main_name = VariableAndHash<std::string>{"main"};
static const auto left_name = VariableAndHash<std::string>{"left"};
static const auto right_name = VariableAndHash<std::string>{"right"};

void TankController::apply() {
    if (std::isnan(surface_power_)) {
        rb_.set_surface_power(main_name, EnginePowerIntent{.surface_power = NAN});
        rb_.set_delta_surface_power(left_name, EnginePowerDeltaIntent::zero());
        rb_.set_delta_surface_power(right_name, EnginePowerDeltaIntent::zero());
    } else {
        float delta_relaxation = std::min(
            steer_relaxation_,
            std::min(std::abs(steer_angle_) / (45.f * degrees), 1.f));
        rb_.set_surface_power(main_name,
            EnginePowerIntent{
                .surface_power = surface_power_,
                .drive_relaxation = drive_relaxation_});
        rb_.set_delta_surface_power(left_name,
            EnginePowerDeltaIntent{
                .delta_power = -sign(steer_angle_) * delta_power_,
                .delta_relaxation = delta_relaxation});
        rb_.set_delta_surface_power(right_name,
            EnginePowerDeltaIntent{
                .delta_power = +sign(steer_angle_) * delta_power_,
                .delta_relaxation = delta_relaxation});
    }
    if (rb_.animation_state_updater_ != nullptr) {
        rb_.animation_state_updater_->notify_movement_intent();
    }
    RigidBodyVehicleController::apply();
}
