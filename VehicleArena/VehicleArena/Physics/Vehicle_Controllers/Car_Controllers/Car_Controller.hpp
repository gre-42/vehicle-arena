// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Interp.hpp>
#include <VehicleArena/Physics/Interfaces/IControllable.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Car_Controllers/Rigid_Body_Vehicle_Controller.hpp>
#include <cstddef>
#include <string>
#include <vector>

namespace VA {

class PhysicsEngine;

class CarController final: public RigidBodyVehicleController, public IControllable {
public:
    CarController(
        RigidBodyVehicle& rb,
        VariableAndHash<std::string> front_engine,
        VariableAndHash<std::string> rear_engine,
        std::vector<size_t> front_tire_ids,
        float max_tire_angle,
        Interp<float> tire_angle_interp,
        PhysicsEngine& physics_engine);
    virtual ~CarController() override;

    virtual void set_stearing_wheel_amount(float left_amount, float relaxation) override;

    // RigidBodyVehicleController
    virtual void apply() override;

    // IControllable
    virtual void notify_reset(const PhysicsEngineConfig& cfg, const PhysicsPhase& phase) override;
private:
    VariableAndHash<std::string> front_engine_;
    VariableAndHash<std::string> rear_engine_;
    std::vector<size_t> front_tire_ids_;
    float max_tire_angle_;
    Interp<float> tire_angle_interp_;
    bool applied_;
    PhysicsEngine& physics_engine_;
};

}
