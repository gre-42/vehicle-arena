// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Interp.hpp>
#include <VehicleArena/Physics/Actuators/Base_Rotor.hpp>
#include <VehicleArena/Physics/Collision/Pacejkas_Magic_Formula.hpp>
#include <string>

namespace VA {

class RigidBodyPulses;
struct NormalImpulse;

/**
 * Represents a tire.
 *
 * References: https://en.wikipedia.org/wiki/Tire_load_sensitivity
 */
class Tire: public BaseRotor {
    Tire(const Tire&) = delete;
    Tire& operator = (const Tire&) = delete;
public:
    Tire(
        const VariableAndHash<std::string>& engine,
        std::optional<VariableAndHash<std::string>> delta_engine,
        RigidBodyPulses* rbp,
        float brake_force,
        float brake_torque,
        float sKs,
        float sKa,
        float sKe,
        const Interp<float>& stiction_coefficient,
        const CombinedPacejkasMagicFormula<float>& magic_formula,
        const FixedArray<float, 3>& vehicle_mount_0,
        const FixedArray<float, 3>& vehicle_mount_1,
        float radius);
    ~Tire();
    void advance_time(float dt);
    FixedArray<float, 3> rotation_axis() const;
    CombinedPacejkasMagicFormula<float> magic_formula;
    float shock_absorber_position;
    float angle_x;
    float angle_y;
    // float accel_x;
    float brake_force;
    float sKs;
    float sKa;
    float sKe;
    Interp<float> stiction_coefficient;
    FixedArray<float, 3> vehicle_mount_0;
    FixedArray<float, 3> vehicle_mount_1;
    FixedArray<float, 3> vertical_line;
    float radius;
    const NormalImpulse* normal_impulse;
};

}
