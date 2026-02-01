// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Sliding_Normal_Modifier.hpp"
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>

using namespace VA;

SlidingNormalModifier::SlidingNormalModifier(
    const RigidBodyPulses& rbp,
    float fac,
    float max_overlap)
    : rbp_{ rbp }
    , fac_{ fac }
    , max_overlap_{ max_overlap }
{}

SlidingNormalModifier::~SlidingNormalModifier() = default;

void SlidingNormalModifier::modify_collision_normal(
    const FixedArray<ScenePos, 3>& position,
    FixedArray<float, 3>& normal,
    float& overlap) const
{
    // auto v = rbp_.velocity_at_position(position);
    auto v = rbp_.v_com_;
    auto lv = std::sqrt(sum(squared(v)));
    if (lv < 1e-12) {
        return;
    }
    v /= lv;
    normal -= fac_ * v * dot0d(normal, v);
    normal /= std::sqrt(sum(squared(normal)));
    overlap = std::min(overlap, max_overlap_);
}
