// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Optimize/Numerical_Differentiation.hpp>
#include <iostream>

namespace VA {

template <class TData, class TFunction, class TGradient>
Array<TData> gradient_descent(
    const Array<TData>& x0,
    const TFunction& func,
    const TGradient& gradient,
    size_t nsteps,
    bool line_search_v1 = false)
{
    Array<TData> x = x0.copy();
    Array<TData> dx;
    TData step_size = 1.f;
    for (size_t i = 0; i < nsteps; ++i) {
        TData f0 = func(x);
        dx = gradient(x);
        // TData len = std::sqrt(sum(squared(dx)));
        // if (len < 1e-12) {
        //     lerr() << "Gradient is zero, stopping after " << i << " steps (f=" << f0 << ")" << sum(squared(x));
        //     return x;
        // }
        // dx /= len;
        step_size *= 2;
        if (line_search_v1) {
            while(!(func(x - step_size * dx) < f0)) {
                // lerr() << "!(" << func(x - step_size * dx) << " < " << f0 << ")";
                step_size /= 2;
                if (step_size < 1e-12) {
                    lerr() <<
                        "Negative gradient does not descent, stopping after " <<
                        i << " steps";
                    return x;
                }
            }
        } else {
            float best_step_size = NAN;
            float best_step_value = NAN;
            while(step_size > 1e-12) {
                TData step_value = func(x - step_size * dx);
                if (std::isnan(best_step_value) || (step_value < best_step_value)) {
                    best_step_size = step_size;
                    best_step_value = step_value;
                } else if (best_step_value < f0) {
                    break;
                }
                step_size /= 2;
            }
            if (!(best_step_value < f0)) {
                // lerr() << "!(" << func(x - step_size * dx) << " < " << f0 << ")";
                lerr() <<
                    "Negative gradient does not descent, stopping after " <<
                    i << " steps";
                return x;
            }
            step_size = best_step_size;
        }
        // lerr() << "step[" << i << "]: " << step_size;
        x -= step_size * dx;
    }
    return x;
}

}
