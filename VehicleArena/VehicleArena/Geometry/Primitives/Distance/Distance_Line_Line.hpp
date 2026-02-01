// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>

#ifdef __GNUC__
#pragma GCC push_options
#pragma GCC optimize ("O3")
#endif

namespace VA {

template <class TData>
bool distance_line_line(
    const FixedArray<TData, 2, 3>& line0,
    const FixedArray<TData, 2, 3>& line1,
    TData& distance)
{
    // From: https://math.stackexchange.com/questions/2213165/find-shortest-distance-between-lines-in-3d
    auto n = cross(line0[1] - line0[0], line1[1] - line1[0]);
    auto l2 = sum(squared(n));
    if (l2 < 1e-12) {
        return false;
    }
    distance = std::abs(dot0d(n, line0[0] - line1[0])) / std::sqrt(l2);
    return true;
}

template <class TData, size_t tndim>
bool distance_line_line(
    const FixedArray<TData, 2, tndim>& line0,
    const FixedArray<TData, 2, tndim>& line1,
    FixedArray<TData, tndim>& p0,
    FixedArray<TData, tndim>& p1)
{
    // From: https://math.stackexchange.com/questions/2213165/find-shortest-distance-between-lines-in-3d
    const auto& a = line0[0];
    const auto& c = line1[0];
    auto b = line0[1] - line0[0];
    auto d = line1[1] - line1[0];
    auto e = line0[0] - line1[0];
    auto b2 = sum(squared(b));
    auto d2 = sum(squared(d));
    auto b2d2 = b2 * d2;
    auto bd = dot0d(b, d);
    auto bd2 = squared(bd);
    auto A = -b2d2 + bd2;
    if (std::abs(A) < 1e-12) {
        return false;
    }
    auto be = dot0d(b, e);
    auto de = dot0d(d, e);
    auto s = (-b2 * de + be * bd) / A;
    auto t = (d2 * be - de * bd) / A;
    if ((t < 0) || (t > 1)) {
        return false;
    }
    if ((s < 0) || (s > 1)) {
        return false;
    }
    p0 = a + b * t;
    p1 = c + d * s;
    return true;
}

}

#ifdef __GNUC__
#pragma GCC pop_options
#endif
