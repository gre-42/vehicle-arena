// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Avatar_As_Car_Controller.hpp"
#include <VehicleArena/Math/Signed_Min.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Advance_Times/Movables/Yaw_Pitch_Look_At_Nodes.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Steering_Type.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_State_Updater.hpp>
#include <stdexcept>

using namespace VA;

AvatarAsCarController::AvatarAsCarController(
    RigidBodyVehicle& rb,
    YawPitchLookAtNodes& ypln,
    float steering_multiplier)
    : RigidBodyVehicleController{ rb, SteeringType::CAR }
    , steering_multiplier_{ steering_multiplier }
    , ypln_{ ypln }
{}

AvatarAsCarController::~AvatarAsCarController() {
    on_destroy.clear();
}

static const auto legs_name = VariableAndHash<std::string>{ "legs" };

void AvatarAsCarController::apply() {
    rb_.set_surface_power(legs_name, EnginePowerIntent{.surface_power = surface_power_}); // NAN=break
    if (!std::isnan(steer_angle_)) {
        ypln_.increment_yaw(steer_angle_ * steering_multiplier_, steer_relaxation_);
    }
    if (rb_.animation_state_updater_ != nullptr) {
        rb_.animation_state_updater_->notify_movement_intent();
    }
    RigidBodyVehicleController::apply();
}
