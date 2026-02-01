// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Missile_Controllers/Rigid_Body_Missile_Controller.hpp>
#include <VehicleArena/Signal/Pid_Controller.hpp>
#include <cstddef>
#include <map>

namespace VA {

struct MissileWingController {
    size_t i;
    FixedArray<float, 2> gain = uninitialized;
    float antiroll_angle;
    float max_angle;
};

class MissileController: public RigidBodyMissileController {
public:
    MissileController(
        RigidBodyVehicle& rb,
        float dt_ref,
        const PidController<FixedArray<float, 2>, float>& pid,
        std::vector<MissileWingController> wing_controllers,
        VariableAndHash<std::string> engine_name);
    virtual ~MissileController() override;
    virtual void apply(float dt) override;
private:
    float dt_ref_;
    PidController<FixedArray<float, 2>, float> pid_;
    std::vector<MissileWingController> wing_controllers_;
    VariableAndHash<std::string> engine_name_;
};

}
