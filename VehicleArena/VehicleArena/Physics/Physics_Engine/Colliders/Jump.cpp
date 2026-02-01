// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Jump.hpp"
#include <VehicleArena/Geometry/Primitives/Vector_At_Position.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>

using namespace VA;

void VA::jump(
    RigidBodyPulses& o0,
    RigidBodyPulses& o1,
    float dv,
    const VectorAtPosition<float, ScenePos, 3>& vp,
    float dt)
{
    if (o0.mass_ == INFINITY) {
        float mc = o1.effective_mass(vp);
        float lambda = - mc * dv;
        o1.integrate_impulse({.vector = -vp.vector * lambda, .position = vp.position}, 0.f, dt, CURRENT_SOURCE_LOCATION);
    } else {
        float mc0 = o0.effective_mass(vp);
        float mc1 = o1.effective_mass(vp);
        float lambda = - (mc0 * mc1 / (mc0 + mc1)) * dv;
        o0.integrate_impulse({.vector = vp.vector * lambda, .position = vp.position}, 0.f, dt, CURRENT_SOURCE_LOCATION);
        o1.integrate_impulse({.vector = -vp.vector * lambda, .position = vp.position}, 0.f, dt, CURRENT_SOURCE_LOCATION);
    }
}
