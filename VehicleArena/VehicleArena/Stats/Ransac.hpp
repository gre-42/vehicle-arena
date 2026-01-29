// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Math/Math.hpp>
#include <Mlib/Stats/Mean.hpp>
#include <Mlib/Stats/RansacOptions.hpp>
#include <Mlib/Stats/Sort.hpp>
#include <Mlib/Throw_Or_Abort.hpp>
#include <algorithm>
#include <limits>
#include <random>

namespace Mlib {

/**
 * Random sampling consensus (RANSAC) algorithm
 *
 * The algorithm can be disabled as follows.
 * - ro.inlier_distance_thresh = infinity
 * - ro.nelems_small >= nelems_large
 * - ro.ncalls = 1
 */
template <class TData, class TCallable>
Array<size_t> ransac(
    size_t nelems_large,
    const RansacOptions<TData>& ro,
    const TCallable& callable)
{
    assert(ro.nelems_small > 0);
    assert(
        (nelems_large >= ro.nelems_small) ||
        ro.inlier_distance_thresh == std::numeric_limits<TData>::infinity());
    Array<size_t> ids_large = arange<size_t>(nelems_large);
    std::mt19937 g(ro.seed);
    size_t best_nonzero = 0;
    Array<size_t> best_indices;

    for (size_t i = 0; i < ro.ncalls; ++i) {

        std::shuffle(ids_large.flat_begin(), ids_large.flat_end(), g);
        Array<size_t> perm(ids_large.flat_begin(), ids_large.flat_begin() + (std::ptrdiff_t)std::min(ro.nelems_small, nelems_large));
        sort(perm);
        Array<TData> positive_residual = substitute_nans(callable(perm), TData(INFINITY));
        if (positive_residual.length() != nelems_large) {
            THROW_OR_ABORT(
                "Residual length (" +
                std::to_string(positive_residual.length())  +
                ")  does not match nelems_large (" +
                std::to_string(nelems_large) + ")");
        }
        Array<bool> also_inliers = (positive_residual <= ro.inlier_distance_thresh);

        if (count_nonzero(also_inliers) > ro.inlier_count_thresh) {
            Array<size_t> better_indices = arange<size_t>(nelems_large)[also_inliers];
            Array<TData> better_positive_residual = substitute_nans(callable(better_indices)[also_inliers], TData(INFINITY));
            size_t better_nonzero = count_nonzero(better_positive_residual <= ro.inlier_distance_thresh);

            // lerr() << "---- " << better_mean_residual << " " << best_mean_residual;
            if (better_nonzero > best_nonzero) {
                best_indices.destroy();
                best_indices = better_indices;
                best_nonzero = better_nonzero;
            }
        }
    }
    return best_indices;
}

}
