// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>

namespace VA {

// From: https://iquilezles.org/articles/ibilinear/

template <class TData>
TData cross(const FixedArray<TData, 2>& a, const FixedArray<TData, 2>& b)
{
    return a(0) * b(1) - a(1) * b(0);
}

template <class TData>
std::optional<FixedArray<TData, 2>> inverse_bilinear(
    const FixedArray<TData, 2>& p,
    const FixedArray<TData, 2>& a,
    const FixedArray<TData, 2>& b,
    const FixedArray<TData, 2>& c,
    const FixedArray<TData, 2>& d)
{
    auto e = b - a;
    auto f = d - a;
    auto g = a - b + c - d;
    auto h = p - a;

    TData k2 = cross(g, f);
    TData k1 = cross(e, f) + cross(h, g);
    TData k0 = cross(h, e);

    if (std::abs(k2) < 0.001f) {
        // if edges are parallel, this is a linear equation
        return FixedArray<TData, 2>{ (h(0) * k1 + f(0) * k0) / (e(0) * k1 - g(0) * k0), -k0 / k1 };
    } else {
        // otherwise, it's a quadratic
        auto w = k1 * k1 - 4.f * k0 * k2;
        if (w < 0) {
            return std::nullopt;
        }
        w = std::sqrt(w);

        auto ik2 = 0.5f / k2;
        auto v = (-k1 - w) * ik2;
        auto u = (h(0) - f(0) * v) / (e(0) + g(0) * v);

        if ((u < 0.0) || (u > 1.f) || (v < 0.f) || (v > (1.f))) {
            v = (-k1 + w) * ik2;
            u = (h(0) - f(0) * v) / (e(0) + g(0) * v);
        }
        return FixedArray<TData, 2>{ u, v };
    }
}

}
