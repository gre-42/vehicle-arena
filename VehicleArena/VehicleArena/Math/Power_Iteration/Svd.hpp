// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Power_Iteration/Power_Iteration.hpp>

namespace VA {

template <class TData>
void svd_u(
    const Array<TData>& a,
    Array<TData>& uT,
    Array<typename FloatType<TData>::value_type>& s,
    Array<TData>& vT,
    size_t seed = 1)
{
    assert(a.ndim() == 2);
    uT.resize(a.shape(0), a.shape(0));
    s.resize(uT.shape(0));
    // A=USV'
    // AA'=US²U'
    // V'=(1/S)U'A
    auto m = outer(a, a);
    for (size_t i = 0; i < uT.shape(0); ++i) {
        power_iteration(m, uT, s(i), i, seed);
    }
    vT = dot(uT, a);
    for (size_t r = 0; r < vT.shape(0); ++r) {
        s(r) = std::sqrt(s(r));
        for (size_t c = 0; c < vT.shape(1); ++c) {
            vT(r, c) /= s(r);
        }
    }
}

template <class TData>
void svd(
    const Array<TData>& a,
    Array<TData>& uT,
    Array<typename FloatType<TData>::value_type>& s,
    Array<TData>& vT,
    size_t seed = 1)
{
    assert(a.ndim() == 2);
    if (a.shape(0) > a.shape(1)) {
        svd_u(a.H(), vT, s, uT, seed);
    } else {
        svd_u(a, uT, s, vT, seed);
    }
}

}
