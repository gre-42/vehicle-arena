// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>

namespace VA {

template <class TData>
bool triangle_is_right_handed(
    const FixedArray<TData, 2>& a,
    const FixedArray<TData, 2>& b,
    const FixedArray<TData, 2>& c)
{
    FixedArray<TData, 2> e0 = b - a;
    FixedArray<TData, 2> e1 = c - b;
    FixedArray<TData, 2> n0{ e0(1), -e0(0) };
    return dot0d(n0, e1) < 0;
}

template <class TData>
bool triangle_is_right_handed(const FixedArray<TData, 3, 2>& triangle)
{
    return triangle_is_right_handed(
        triangle[0],
        triangle[1],
        triangle[2]);
}

}
