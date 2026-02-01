// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Physics/Vehicle_Controllers/Plane_Controllers/Rigid_Body_Plane_Controller.hpp>
#include <cstddef>
#include <vector>

namespace VA {

class PlaneController: public RigidBodyPlaneController {
public:
    PlaneController(
        RigidBodyVehicle& rb,
        std::vector<size_t> left_front_aileron_wing_ids,
        std::vector<size_t> right_front_aileron_wing_ids,
        std::vector<size_t> left_rear_aileron_wing_ids,
        std::vector<size_t> right_rear_aileron_wing_ids,
        std::vector<size_t> left_rudder_wing_ids,
        std::vector<size_t> right_rudder_wing_ids,
        std::vector<size_t> left_flap_wing_ids,
        std::vector<size_t> right_flap_wing_ids);
    virtual ~PlaneController() override;
    virtual void apply() override;
private:
    std::vector<size_t> left_front_aileron_wing_ids_;
    std::vector<size_t> right_front_aileron_wing_ids_;
    std::vector<size_t> left_rear_aileron_wing_ids_;
    std::vector<size_t> right_rear_aileron_wing_ids_;
    std::vector<size_t> left_rudder_wing_ids_;
    std::vector<size_t> right_rudder_wing_ids_;
    std::vector<size_t> left_flap_wing_ids_;
    std::vector<size_t> right_flap_wing_ids_;
};

}
