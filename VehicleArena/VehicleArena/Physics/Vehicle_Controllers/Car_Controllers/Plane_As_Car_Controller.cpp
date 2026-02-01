// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Plane_As_Car_Controller.hpp"
#include <VehicleArena/Math/Signed_Min.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Physics/Rigid_Body/Vehicle_Domain.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Steering_Type.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_State_Updater.hpp>
#include <stdexcept>

using namespace VA;

PlaneAsCarController::PlaneAsCarController(
    RigidBodyVehicle& rb,
    const std::map<size_t, float>& tire_angles)
    : RigidBodyVehicleController{ rb, SteeringType::CAR }
    , tire_angles_{ tire_angles }
{
    ascend_to(rb.rbp_.abs_position()(1));
}

PlaneAsCarController::~PlaneAsCarController() {
    on_destroy.clear();
}

static const auto wheels_name = VariableAndHash<std::string>{"wheels"};
static const auto turbine_name = VariableAndHash<std::string>{"turbine"};

void PlaneAsCarController::apply_this() {
    auto forward = [this](){
        for (const auto& [tire_id, _] : tire_angles_) {
            rb_.set_tire_angle_y(tire_id, 0.f);
        }};
    auto steer = [this](){
        for (const auto& [tire_id, max_angle] : tire_angles_) {
            float ang = signed_min(steer_angle_ * steer_relaxation_, max_angle);
            rb_.set_tire_angle_y(tire_id, ang);
        }};
    switch (rb_.current_vehicle_domain_) {
    case VehicleDomain::AIR:
    case VehicleDomain::UNDEFINED:
        forward();
        rb_.set_surface_power(wheels_name, EnginePowerIntent{ .surface_power = NAN });  // NAN=break
        rb_.set_surface_power(turbine_name, EnginePowerIntent{.surface_power = surface_power_});
        return;
    case VehicleDomain::GROUND:
        steer();
        rb_.set_surface_power(wheels_name, EnginePowerIntent{.surface_power = std::isnan(surface_power_) ? NAN : 0.f});
        rb_.set_surface_power(turbine_name, EnginePowerIntent{.surface_power = surface_power_});
        return;
    case VehicleDomain::END:
        throw std::runtime_error("Invalid vehicle domain");
    }
    throw std::runtime_error("Unknown vehicle domain");
}

void PlaneAsCarController::apply() {
    apply_this();
    if (rb_.animation_state_updater_ != nullptr) {
        rb_.animation_state_updater_->notify_movement_intent();
    }
    RigidBodyVehicleController::apply();
}
