// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Attached_Wheel.hpp"
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>

using namespace VA;

AttachedWheel::AttachedWheel(
    const RigidBodyPulses& vehicle,
    RigidBodyPulses& wheel,
    const FixedArray<float, 3>& vertical_line)
    : vehicle_{ vehicle }
    , wheel_{ wheel }
    , vertical_line_{ vertical_line }
{}

FixedArray<float, 3> AttachedWheel::velocity_at_position(const FixedArray<ScenePos, 3>& position) const {
    auto vv = vehicle_.velocity_at_position(position);
    vv += vertical_line_ * dot0d(vertical_line_, wheel_.v_com_ - vv);
    return vv;
}

float AttachedWheel::effective_mass(const VectorAtPosition<float, ScenePos, 3>& vp) const {
    return wheel_.effective_mass(vp);
}

void AttachedWheel::integrate_impulse(const VectorAtPosition<float, ScenePos, 3>& J, float extra_w, float dt, const SourceLocation& loc) {
    wheel_.integrate_impulse(J, extra_w, dt, loc);
}
