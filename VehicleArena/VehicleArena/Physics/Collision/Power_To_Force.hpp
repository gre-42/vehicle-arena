// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>

namespace VA {

FixedArray<float, 3> min_l2(const FixedArray<float, 3>& v, float max_length);

/**
 * solve([1/2*m*((v+F/m*t)^2-v^2) + 1/2*M*((V-F/M*t)^2-V^2)=P*t, m*v + M*V = m*(v+F/m*t) + M*(V-F/M*t)], F);
 */
void power_to_forces_finite_masses(
    FixedArray<float, 3>& F0,
    FixedArray<float, 3>& F1,
    const FixedArray<float, 3>& power3,
    float M,
    float m,
    const FixedArray<float, 3>& V3,
    const FixedArray<float, 3>& v3,
    float dt);

/**
 * solve(1/2*m*((v+F/m*t)^2-v^2) = P*t, F);
 * 
 * P == NAN => brake
 */
FixedArray<float, 3> power_to_force_infinite_mass(
    float break_accel,
    float hand_brake_velocity,
    float max_stiction_force,
    float friction_force,
    float max_velocity,
    const FixedArray<float, 3>& n3,
    float P,
    const FixedArray<float, 3>& v3,
    float dt,
    float alpha0,
    bool avoid_burnout);

VA::FixedArray<float, 3> friction_force_infinite_mass(
    float max_stiction_force,
    float friction_force,
    const FixedArray<float, 3>& v3,
    float alpha0);

}
