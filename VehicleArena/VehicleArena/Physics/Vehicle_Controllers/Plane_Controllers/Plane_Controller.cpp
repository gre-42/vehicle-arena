// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Plane_Controller.hpp"
#include <VehicleArena/Math/Signed_Min.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Steering_Type.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_State_Updater.hpp>
#include <stdexcept>

using namespace VA;

PlaneController::PlaneController(
    RigidBodyVehicle& rb,
    std::vector<size_t> left_front_aileron_wing_ids,
    std::vector<size_t> right_front_aileron_wing_ids,
    std::vector<size_t> left_rear_aileron_wing_ids,
    std::vector<size_t> right_rear_aileron_wing_ids,
    std::vector<size_t> left_rudder_wing_ids,
    std::vector<size_t> right_rudder_wing_ids,
    std::vector<size_t> left_flap_wing_ids,
    std::vector<size_t> right_flap_wing_ids)
    : RigidBodyPlaneController{ rb, SteeringType::CAR }
    , left_front_aileron_wing_ids_{ std::move(left_front_aileron_wing_ids) }
    , right_front_aileron_wing_ids_{ std::move(right_front_aileron_wing_ids) }
    , left_rear_aileron_wing_ids_{ std::move(left_rear_aileron_wing_ids) }
    , right_rear_aileron_wing_ids_{ std::move(right_rear_aileron_wing_ids) }
    , left_rudder_wing_ids_{ std::move(left_rudder_wing_ids) }
    , right_rudder_wing_ids_{ std::move(right_rudder_wing_ids) }
    , left_flap_wing_ids_{ std::move(left_flap_wing_ids) }
    , right_flap_wing_ids_{ std::move(right_flap_wing_ids) }
{}

PlaneController::~PlaneController() = default;

static const auto turbine_name = VariableAndHash<std::string>{ "turbine" };

void PlaneController::apply() {
    rb_.set_surface_power(turbine_name, EnginePowerIntent{
        .surface_power = turbine_power_,
        .drive_relaxation = throttle_relaxation_ });
    for (size_t i : left_front_aileron_wing_ids_) {
        rb_.set_wing_angle_of_attack(i, -pitch_amount_ * pitch_relaxation_ + roll_amount_ * roll_relaxation_);
    }
    for (size_t i : right_front_aileron_wing_ids_) {
        rb_.set_wing_angle_of_attack(i, -pitch_amount_ * pitch_relaxation_ - roll_amount_ * roll_relaxation_);
    }
    for (size_t i : left_rear_aileron_wing_ids_) {
        rb_.set_wing_angle_of_attack(i, pitch_amount_ * pitch_relaxation_ + roll_amount_ * roll_relaxation_);
    }
    for (size_t i : right_rear_aileron_wing_ids_) {
        rb_.set_wing_angle_of_attack(i, pitch_amount_ * pitch_relaxation_ - roll_amount_ * roll_relaxation_);
    }
    for (size_t i : left_rudder_wing_ids_) {
        rb_.set_wing_angle_of_attack(i, yaw_amount_ * yaw_relaxation_);
    }
    for (size_t i : right_rudder_wing_ids_) {
        rb_.set_wing_angle_of_attack(i, yaw_amount_ * yaw_relaxation_);
    }
    for (size_t i : left_flap_wing_ids_) {
        rb_.set_wing_brake_angle(i, brake_amount_ * throttle_relaxation_);
    }
    for (size_t i : right_flap_wing_ids_) {
        rb_.set_wing_brake_angle(i, brake_amount_ * throttle_relaxation_);
    }
    if (rb_.animation_state_updater_ != nullptr) {
        rb_.animation_state_updater_->notify_movement_intent();
    }
}
