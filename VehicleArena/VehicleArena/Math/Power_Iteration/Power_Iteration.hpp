// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>

namespace VA {

/*
 * Power iteration on a symmetric matrix.
 * Eigenvector is orthogonalized w.r.t. orthonormal v[:i].
 * Eigenvalue is stored in s.
 */
template <class TData>
void power_iteration(
    const Array<TData>& a,
    Array<TData>& uT,
    typename FloatType<TData>::value_type& s,
    size_t i,
    size_t seed = 1)
{
    assert(a.ndim() == 2);
    assert(a.shape(0) == a.shape(1));
    randomize_array(uT[i], seed);

    for (size_t n = 0; n < 30 * a.shape(0); n++) {
        for (size_t r = 0; r < i; ++r) {
            uT[i] -= uT[r] * outer(uT[i], uT[r])();
        }
        Array<TData> ui_old;
        ui_old = uT[i];
        uT[i] = outer(uT[i], a);
        typename FloatType<TData>::value_type s_old = s;
        s = std::sqrt(sum(norm(uT[i])));
        if (s < 1e-12) {
            // undo, and abort, because diff will be 0
            uT[i] = ui_old;
        } else {
            uT[i] /= s;
        }

        {
            Array<TData> diff = uT[i] - ui_old;
            auto diff_norm = sum(norm(diff));
            if (diff_norm < 1e-12) {
                return;
            }
        }

        {
            Array<TData> diff = uT[i] + ui_old;
            auto diff_norm = sum(norm(diff));
            if (diff_norm < 1e-12) {
                s = -s;
                return;
            }
        }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        // Required for close-to-identical eigenvalues.
        if ((n > 0) && (std::abs(s - s_old) < 1e-7)) {
            return;
        }
#pragma GCC diagnostic pop
    }
    throw PowerIterationDidNotConvergeError("power iteration did not converge");
}

}
