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
template <typename TData, size_t... tshape>
class FixedArray;

class RigidBodyAvatarController {
public:
    RigidBodyAvatarController();
    virtual ~RigidBodyAvatarController();
    void increment_legs_z(const FixedArray<float, 3>& dz);
    void walk(float surface_power, float relaxation);
    void stop();
    void set_target_yaw(float target_yaw);
    void set_target_pitch(float target_pitch);
    void increment_yaw(float dyaw, float relaxation);
    void increment_pitch(float dpitch, float relaxation);
    void reset();
    virtual void apply() = 0;
protected:
    FixedArray<float, 3> legs_z_;
    float target_yaw_;
    float target_pitch_;
    float dyaw_;
    float dyaw_relaxation_;
    float dpitch_;
    float dpitch_relaxation_;
    float surface_power_;
    float drive_relaxation_;
};

}
