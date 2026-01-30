// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>
#include <algorithm>

namespace VA {

template <class TData>
TData clipped_element(const TData& v, const TData& low, const TData& high) {
    if (std::isnan(v)) {
        return v;
    } else {
        return std::clamp(v, low, high);
    }
}

template <class TData>
void clip(Array<TData>& a, const TData& low, const TData& high) {
    for (auto& v : a.flat_iterable()) {
        v = clipped_element(v, low, high);
    }
}

template <class TData>
Array<TData> clipped(const Array<TData>& a, const TData& low, const TData& high) {
    Array<TData> result = a.copy();
    clip(result, low, high);
    return result;
}

template <class TData>
void normalize_and_clip(Array<TData>& a, const TData& low, const TData& high) {
    a -= low;
    a /= (high - low);
    clip<TData>(a, 0, 1);
}

template <class TData>
Array<TData> normalized_and_clipped(const Array<TData>& a, const TData& low, const TData& high) {
    Array<TData> result = a.copy();
    normalize_and_clip(result, low, high);
    return result;
}

template <class TData>
void normalize_and_clip(Array<TData>& a) {
    Array<TData> va = a[VA::isfinite(a)];
    normalize_and_clip(a, min(va), max(va));
}

template <class TData>
Array<TData> normalized_and_clipped(const Array<TData>& a) {
    Array<TData> result = a.copy();
    normalize_and_clip(result);
    return result;
}

}
