// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>

namespace VA {

template <class TData>
void inverse_iteration_symm(
    const Array<TData>& a,
    Array<TData>& u,
    typename FloatType<TData>::value_type& s,
    const TData& alpha = 0,
    const TData& beta = 0,
    unsigned int seed = 1)
{
    assert(a.ndim() == 2);
    assert(a.shape(0) == a.shape(1));
    u = random_array3<TData>(ArrayShape{a.shape(0)}, seed);

    for (size_t n = 0; n < 30 * a.shape(0); n++) {
        // u = lstsq_chol_1d(a, u, float(1e-1));
        u = solve_symm_1d(a, u, alpha, beta).value();
        s = 1 / std::sqrt(sum(norm(u)));
        u *= s;
    }
}

}
