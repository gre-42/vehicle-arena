// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Rigid_Body_Missile_Controller.hpp"
#include <VehicleArena/Math/Fixed_Math.hpp>

using namespace VA;

RigidBodyMissileController::RigidBodyMissileController(RigidBodyVehicle& rb)
    : rb_{ rb }
    , rocket_engine_power_{ NAN }
    , rocket_engine_power_relaxation_{ 0.f }
    , desired_direction_{ fixed_nans<float, 3>() }
    , desired_direction_relaxation_{ 0.f }
{}

RigidBodyMissileController::~RigidBodyMissileController() = default;

void RigidBodyMissileController::set_desired_direction(
    const FixedArray<float, 3>& dir,
    float relaxation)
{
    if (relaxation >= desired_direction_relaxation_) {
        auto l2 = sum(squared(dir));
        if (std::abs(l2 - 1.f) > 1e6f) {
            throw std::runtime_error("Desired missile direction not normalized. Length: " + std::to_string(std::sqrt(l2)));
        }
        desired_direction_ = dir;
        desired_direction_relaxation_ = relaxation;
    }
}

void RigidBodyMissileController::reset_parameters() {
    rocket_engine_power_ = NAN;
    desired_direction_ = NAN;
}

void RigidBodyMissileController::reset_relaxation() {
    rocket_engine_power_relaxation_ = 0.f;
    desired_direction_relaxation_ = 0.f;
}

void RigidBodyMissileController::throttle_engine(
    float rocket_engine_power,
    float relaxation)
{
    if (relaxation > rocket_engine_power_relaxation_) {
        rocket_engine_power_ = rocket_engine_power;
        rocket_engine_power_relaxation_ = relaxation;
    }
}
