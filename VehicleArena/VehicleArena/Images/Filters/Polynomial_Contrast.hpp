// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Images/Mesh_Coordinates/Vandermonde.hpp>
#include <VehicleArena/Math/Math.hpp>

namespace VA {

template <class TData>
Array<TData> polynomial_contrast(const Array<Array<TData>>& x, const Array<TData>& weights, const Array<TData>& contrast, size_t poly_degree) {
    assert(x.ndim() == 1);
    assert(all(x(0).shape() == weights.shape()));
    Array<Array<TData>> V = vanderNd(x, poly_degree);
    assert(V.length() == contrast.length());
    Array<TData> M{ ArrayShape{ V.length(), V.length() }};
    for (size_t r = 0; r < M.shape(0); ++r) {
        for (size_t c = 0; c < M.shape(0); ++c) {
            if (r <= c) {
                M(r, c) = sum(V(r) * V(c) * weights);
            } else {
                M(r, c) = M(c, r);
            }
        }
    }
    Array<TData> m = solve_symm_1d(M, contrast).value();
    Array<TData> result = full<TData>(x(0).shape(), m(0));
    for (size_t i = 1; i < V.shape(0); ++i) {
        result += m(i) * V(i);
    }
    result *= weights;
    return result;
}

}
