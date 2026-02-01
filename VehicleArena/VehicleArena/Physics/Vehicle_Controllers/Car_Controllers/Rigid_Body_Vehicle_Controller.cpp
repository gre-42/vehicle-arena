// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Rigid_Body_Vehicle_Controller.hpp"
#include <VehicleArena/Physics/Units.hpp>
#include <VehicleArena/Threads/Recursion_Guard.hpp>
#include <VehicleArena/Threads/Thread_Local.hpp>
#include <cmath>
#include <stdexcept>

using namespace VA;

RigidBodyVehicleController::RigidBodyVehicleController(
    RigidBodyVehicle& rb,
    SteeringType steering_type)
    : steering_type{ steering_type }
    , rb_{ rb }
    , surface_power_{ NAN }
    , drive_relaxation_{ 0.f }
    , steer_angle_{ NAN }
    , steer_relaxation_{ 0.f }
    , target_height_{ NAN }
    , trailer_{ nullptr }
{}

RigidBodyVehicleController::~RigidBodyVehicleController() = default;

void RigidBodyVehicleController::step_on_brakes(float relaxation) {
    if (relaxation < drive_relaxation_) {
        return;
    }
    surface_power_ = NAN;
    drive_relaxation_ = relaxation;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->step_on_brakes(relaxation);
    }
}

void RigidBodyVehicleController::drive(float surface_power, float relaxation) {
    if (relaxation < drive_relaxation_) {
        return;
    }
    surface_power_ = surface_power;
    drive_relaxation_ = relaxation;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->drive(surface_power, relaxation);
    }
}

void RigidBodyVehicleController::roll_tires() {
    surface_power_ = 0.f;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->roll_tires();
    }
}

void RigidBodyVehicleController::steer(float angle, float relaxation) {
    if (relaxation < steer_relaxation_) {
        return;
    }
    steer_angle_ = angle;
    steer_relaxation_ = relaxation;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->steer(angle, relaxation);
    }
}

void RigidBodyVehicleController::set_stearing_wheel_amount(float left_amount, float relaxation) {
    steer(left_amount * 45.f * degrees, relaxation);
}

void RigidBodyVehicleController::ascend_to(double target_height) {
    target_height_ = target_height;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->ascend_to(target_height);
    }
}

void RigidBodyVehicleController::ascend_by(double delta_height) {
    if (std::isnan(target_height_)) {
        return;
    }
    target_height_ += delta_height;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->ascend_by(delta_height);
    }
}

void RigidBodyVehicleController::reset_parameters(
    float surface_power,
    float steer_angle)
{
    surface_power_ = surface_power;
    steer_angle_ = steer_angle;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->reset_parameters(surface_power, steer_angle);
    }
}

void RigidBodyVehicleController::reset_relaxation(
    float drive_relaxation,
    float steer_relaxation)
{
    drive_relaxation_ = drive_relaxation;
    steer_relaxation_ = steer_relaxation;
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->reset_relaxation(drive_relaxation, steer_relaxation);
    }
}

void RigidBodyVehicleController::set_trailer(
    const DanglingBaseClassRef<RigidBodyVehicleController>& trailer)
{
    if (trailer_ != nullptr) {
        throw std::runtime_error("Trailer already set (0)");
    }
    if (on_destroy_trailer_.has_value()) {
        throw std::runtime_error("Trailer already set (1)");
    }
    on_destroy_trailer_.emplace(trailer->on_destroy, CURRENT_SOURCE_LOCATION);
    on_destroy_trailer_->add([this](){
        trailer_ = nullptr;
        on_destroy_trailer_.reset();
    }, CURRENT_SOURCE_LOCATION);
    trailer_ = trailer.ptr();
}

void RigidBodyVehicleController::apply() {
    if (trailer_ != nullptr) {
        static THREAD_LOCAL(RecursionCounter) recursion_counter = RecursionCounter{};
        RecursionGuard rg{recursion_counter};
        trailer_->apply();
    }
}
