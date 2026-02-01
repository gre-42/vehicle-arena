// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Memory/Destruction_Notifier.hpp>
#include <optional>

namespace VA {

class RigidBodyVehicle;
enum class SteeringType;

class RigidBodyVehicleController: public virtual DanglingBaseClass, public virtual DestructionNotifier {
public:
    RigidBodyVehicleController(
        RigidBodyVehicle& rb,
        SteeringType steering_type);
    virtual ~RigidBodyVehicleController();
    void step_on_brakes(float relaxation);
    void drive(float surface_power, float relaxation);
    void roll_tires();
    void steer(float angle, float relaxation);
    virtual void set_stearing_wheel_amount(float left_amount, float relaxation);
    void ascend_to(double target_height);
    void ascend_by(double delta_height);
    void reset_parameters(
        float surface_power,
        float steer_angle);
    void reset_relaxation(
        float drive_relaxation,
        float steer_relaxation);
    void set_trailer(const DanglingBaseClassRef<RigidBodyVehicleController>& trailer);
    virtual void apply();
    const SteeringType steering_type;
protected:
    RigidBodyVehicle& rb_;
    float surface_power_;
    float drive_relaxation_;
    float steer_angle_;
    float steer_relaxation_;
    double target_height_;
private:
    DanglingBaseClassPtr<RigidBodyVehicleController> trailer_;
    std::optional<DestructionFunctionsRemovalTokens> on_destroy_trailer_;
};

}
