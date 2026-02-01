// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once

namespace VA {

class RigidBodyVehicle;
enum class SteeringType;

class RigidBodyPlaneController {
public:
    RigidBodyPlaneController(
        RigidBodyVehicle& rb,
        SteeringType steering_type);
    virtual ~RigidBodyPlaneController();
    void brake(float amount, float relaxation);
    void accelerate(float turbine_power, float relaxation);
    void pitch(float amount, float relaxation);
    void yaw(float amount, float relaxation);
    void roll(float amount, float relaxation);
    void reset_parameters(
        float turbine_power,
        float brake_amount,
        float pitch_amount,
        float yaw_amount,
        float roll_amount);
    void reset_relaxation(
        float throttle_relaxation,
        float pitch_relaxation,
        float yaw_relaxation,
        float roll_relaxation);
    virtual void apply() = 0;
    const SteeringType steering_type;
protected:
    RigidBodyVehicle& rb_;
    float turbine_power_;
    float brake_amount_;
    float throttle_relaxation_;
    float pitch_amount_;
    float pitch_relaxation_;
    float yaw_amount_;
    float yaw_relaxation_;
    float roll_amount_;
    float roll_relaxation_;
};

}
