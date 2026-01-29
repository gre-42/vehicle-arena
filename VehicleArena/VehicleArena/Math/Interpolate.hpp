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
Array<TData> interpolate(const Array<TData>& a, const Array<TData>& y) {
    return a.applied([&](const TData& x){
        if (std::isnan(x)) {
            return NAN;
        }
        size_t left_id = (size_t)std::lround(std::floor(x));
        size_t right_id = left_id + 1;
        if (right_id == y.length() && x <= TData(y.length() - 1)) {
            return y(left_id);
        } else if (left_id < y.length() && right_id < y.length()) {
            TData h = x - (TData)left_id;
            return y(left_id) * (1 - h) + y(right_id) * h;
        } else {
            return NAN;
        }
    });
}

}
