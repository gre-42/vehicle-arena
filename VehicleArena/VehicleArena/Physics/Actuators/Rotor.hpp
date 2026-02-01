// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Actuators/Base_Rotor.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Signal/Pid_Controller.hpp>
#include <string>

namespace VA {

struct StaticWorld;

enum class GravityCorrection {
    NONE,
    GIMBAL,
    MOVE
};

GravityCorrection gravity_correction_from_string(const std::string& str);

class Rotor: public BaseRotor {
public:
    Rotor(
        const VariableAndHash<std::string>& engine,
        const std::optional<VariableAndHash<std::string>>& delta_engine,
        const TransformationMatrix<float, ScenePos, 3>& rest_location,
        float power2lift,
        float w,
        GravityCorrection gravity_correction,
        float radius,
        float max_align_to_gravity,
        const PidController<float, float>& align_to_gravity_pid_x,
        const PidController<float, float>& align_to_gravity_pid_y,
        float drift_reduction_factor,
        float drift_reduction_reference_velocity,
        const FixedArray<float, 3>& vehicle_mount_0,
        const FixedArray<float, 3>& vehicle_mount_1,
        const FixedArray<float, 3>& blades_mount_0,
        const FixedArray<float, 3>& blades_mount_1,
        RigidBodyPulses* rotor_rb);
    Rotor(const Rotor&) = delete;
    Rotor& operator = (const Rotor&) = delete;
    ~Rotor();
    TransformationMatrix<float, ScenePos, 3> rotated_location(
        const TransformationMatrix<float, ScenePos, 3>& parent_location,
        const FixedArray<float, 3>& parent_velocity,
        const StaticWorld& static_world);
    FixedArray<float, 3> rotation_axis() const;
    TransformationMatrix<float, ScenePos, 3> rest_location;
    FixedArray<float, 3> angles;
    FixedArray<ScenePos, 3> movement;
    float power2lift;
    float w;
    FixedArray<float, 3> vehicle_mount_0;
    FixedArray<float, 3> vehicle_mount_1;
    FixedArray<float, 3> blades_mount_0;
    FixedArray<float, 3> blades_mount_1;
private:
    GravityCorrection gravity_correction_;
    ScenePos radius_;
    float max_align_to_gravity_;
    PidController<float, float> align_to_gravity_pid_x_;
    PidController<float, float> align_to_gravity_pid_y_;
    float drift_reduction_factor_;
    float drift_reduction_reference_velocity_;
};

}
