// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>

namespace VA {

template <class TData, class Tf>
Array<TData> numerical_differentiation(const Tf& f, const Array<TData>& x, const TData& h=1e-3) {
    auto fx = f(x);

    Array<TData> JT(ArrayShape{x.length(), fx.length()});
    for (size_t i = 0; i < x.length(); ++i) {
        Array<TData> xi;
        xi = x;
        xi(i) += h;
        JT[i] = (f(xi) - fx) / h;
    }
    return JT.T();
}

template <size_t c, class TData, class Tf>
Array<TData> mixed_numerical_differentiation(const Tf& f, const FixedArray<TData, c>& x, const TData& h = 1e-3) {
    auto fx = f(x);

    Array<TData> JT(ArrayShape{ x.length(), fx.length() });
    for (size_t i = 0; i < x.length(); ++i) {
        FixedArray<TData, c> xi = x;
        xi(i) += h;
        JT[i] = (f(xi) - fx) / h;
    }
    return JT.T();
}

template <size_t r, size_t c, class TData, class Tf>
FixedArray<TData, r, c> numerical_differentiation(const Tf& f, const FixedArray<TData, c>& x, const TData& h = 1e-3) {
    FixedArray<TData, r> fx = f(x);

    FixedArray<TData, c, r> JT = uninitialized;
    for (size_t i = 0; i < x.length(); ++i) {
        FixedArray<TData, c> xi = x;
        xi(i) += h;
        JT[i] = (f(xi) - fx) / h;
    }
    return JT.T();
}

}
