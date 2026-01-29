// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Math/Math.hpp>
#include <Mlib/Stats/Sort.hpp>

namespace Mlib {

template <class TData>
TData median(const Array<TData>& a) {
    if (a.length() == 0) {
        return NAN;
    } else {
        Array<TData> af = sorted(a);
        return af(af.length() / 2);
    }
}

template <class TData>
TData median_inplace(Array<TData>& a) {
    if (a.length() == 0) {
        return NAN;
    } else {
        sort(a);
        return a(a.length() / 2);
    }
}

template <class TData>
TData nanmedian(const Array<TData>& a) {
    return median(a[!Mlib::isnan(a)]);
}

/**
 * Median absolute deviation.
 * Based on statsmodels.robust.scale.mad.
 * 0.6744897501960817 = norm.ppf(3 / 4)
 */
template <class TData>
TData mad(const Array<TData>& a, TData* median = nullptr, TData c = TData(0.674)) {
    TData me = ::Mlib::median(a);
    TData ma = ::Mlib::median(abs(a - me)) / c;
    if (median != nullptr) {
        *median = me;
    }
    return ma;
}

template <class TData>
Array<TData> robust_deviation(const Array<TData>& sample) {
    TData median;
    TData mad = ::Mlib::mad(sample, &median);
    return (sample - median) / mad;
}

}
