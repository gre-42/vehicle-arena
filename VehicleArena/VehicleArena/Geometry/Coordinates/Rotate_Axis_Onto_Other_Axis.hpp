// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>

namespace VA {

/** Rotate a matrix s.t. a specific axis is on another axis afterwards.
 */
template <class TData>
FixedArray<TData, 3, 3> rotate_axis_onto_other_axis(
    const FixedArray<TData, 3, 3>& R,
    const FixedArray<TData, 3>& abs_fixed_axis,
    const FixedArray<TData, 3>& rel_rotating_axis,
    float relaxation = 1)
{
    FixedArray<TData, 3> g = dot(abs_fixed_axis, R);
    // Find the axis that can rotate the "abs_fixed_axis" vector onto the "abs_fixed_axis".
    FixedArray<TData, 3> d = cross(rel_rotating_axis, g);
    TData d_len2 = sum(squared(d));
    if (d_len2 > 1e-12) {
        TData d_len = std::sqrt(d_len2);
        d /= d_len;
        TData ang = relaxation * std::asin(std::clamp(d_len, 0.f, 1.f));
        return dot2d(R, rodrigues2(d, ang));
    } else {
        // Abort if we are already aligned to the target axis.
        return R;
    }
}

}
