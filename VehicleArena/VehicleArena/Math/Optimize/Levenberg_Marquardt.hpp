// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Cholesky.hpp>
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Math/Optimize/Generic_Optimization.hpp>
#include <optional>

namespace VA {

template <class TData, class TdAT_A = Array<TData>, class TdAT_b = Array<TData>, class TX, class TF, class TJacobian>
TX levenberg_marquardt(
    const TX& x0,
    const Array<TData>& y,
    const TF& f,
    const TJacobian& J,
    const TData& alpha,
    const TData& beta,
    const TData& alpha2,
    const TData& beta2,
    const TData& min_redux,
    size_t niterations = 100,
    size_t nburnin = 5,
    size_t nmisses = 3,
    bool print_residual = true,
    bool nothrow = false,
    Array<TData>* final_residual = nullptr,
    const TData* max_residual = nullptr,
    const TdAT_A* dAT_A = nullptr,
    const TdAT_b* dAT_b = nullptr)
{
    return generic_optimization(
        x0,
        [&](const TX& x, size_t i){
            return y - f(x);
        },
        [](const TX& x, const Array<TData>& residual, size_t i){
            return (residual.length() == 0)
                ? 0
                : sum(squared(residual)) / residual.length();
        },
        [&](const TX& x, const Array<TData>& residual, size_t i){
            std::optional<TX> dx;
            lstsq_chol_1d(
                dx,
                J(x),
                residual,
                i < nburnin ? alpha : alpha2,
                i < nburnin ? beta : beta2,
                dAT_A,
                dAT_b);
            return x + dx.value();
        },
        min_redux,
        niterations,
        nmisses,
        print_residual,
        nothrow,
        final_residual,
        max_residual);
}

}
