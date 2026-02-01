// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Car_Controller.hpp"
#include <VehicleArena/Math/Signed_Min.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Physics_Engine/Physics_Engine.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Steering_Type.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_State_Updater.hpp>
#include <stdexcept>

using namespace VA;

CarController::CarController(
    RigidBodyVehicle& rb,
    VariableAndHash<std::string> front_engine,
    VariableAndHash<std::string> rear_engine,
    std::vector<size_t> front_tire_ids,
    float max_tire_angle,
    Interp<float> tire_angle_interp,
    PhysicsEngine& physics_engine)
    : RigidBodyVehicleController{ rb, SteeringType::CAR }
    , front_engine_{ front_engine }
    , rear_engine_{ rear_engine }
    , front_tire_ids_{ std::move(front_tire_ids) }
    , max_tire_angle_{ max_tire_angle }
    , tire_angle_interp_{ std::move(tire_angle_interp) }
    , applied_{ false }
    , physics_engine_{ physics_engine }
{
    physics_engine_.add_controllable(*this);
}

CarController::~CarController()
{
    on_destroy.clear();
    physics_engine_.remove_controllable(*this);
}

void CarController::set_stearing_wheel_amount(float left_amount, float relaxation) {
    float v = std::abs(dot0d(
        rb_.rbp_.v_com_,
        rb_.rbp_.rotation_.column(2)));
    steer(left_amount * tire_angle_interp_(v), relaxation);
}

void CarController::apply() {
    if (applied_) {
        throw std::runtime_error("Car controller already applied");
    }
    applied_ = true;
    rb_.set_surface_power(front_engine_, EnginePowerIntent{
        .surface_power = surface_power_,
        .drive_relaxation = drive_relaxation_});   // NAN=break
    if (front_engine_ != rear_engine_) {
        rb_.set_surface_power(rear_engine_, EnginePowerIntent{
            .surface_power = surface_power_,
            .drive_relaxation = drive_relaxation_}); // NAN=break
    }
    if (!front_tire_ids_.empty()) {
        float ang = signed_min(steer_angle_ * steer_relaxation_, max_tire_angle_);
        for (size_t tire_id : front_tire_ids_) {
            rb_.set_tire_angle_y(tire_id, ang);
        }
    }
    if (rb_.animation_state_updater_ != nullptr) {
        rb_.animation_state_updater_->notify_movement_intent();
    }
    RigidBodyVehicleController::apply();
}

void CarController::notify_reset(const PhysicsEngineConfig& cfg, const PhysicsPhase& phase) {
    applied_ = false;
}
