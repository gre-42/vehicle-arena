// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>

namespace VA {

class RigidBodyVehicle;

class RigidBodyMissileController {
public:
    explicit RigidBodyMissileController(RigidBodyVehicle& rb);
    virtual ~RigidBodyMissileController();
    void set_desired_direction(
        const FixedArray<float, 3>& dir,
        float relaxation);
    void reset_parameters();
    void reset_relaxation();
    void throttle_engine(
        float rocket_engine_power,
        float relaxation);
    virtual void apply(float dt) = 0;
protected:
    RigidBodyVehicle& rb_;
    float rocket_engine_power_;
    float rocket_engine_power_relaxation_;
    FixedArray<float, 3> desired_direction_;
    float desired_direction_relaxation_;
};

}
