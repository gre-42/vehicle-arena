// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Stats/Cdf.hpp>
#include <VehicleArena/Stats/Quantile.hpp>

namespace VA {

template <class TData, class TDataRef, class TFloat=TData>
class HistogramMatching {
public:
    HistogramMatching(const Array<TData>& data, const Array<TDataRef>& data_ref, size_t nbins = 10)
    : cdf_{data, nbins},
      quantiles_{data_ref}
    {}
    const TData& operator () (const TData& v, bool check_bounds = false) {
        return quantiles_(cdf_(v, check_bounds), TFloat{0});
    }
private:
    Cdf<TData, TFloat> cdf_;
    Quantiles<TData> quantiles_;
};

template <class TData, class TDataRef, class TFloat=TData>
Array<TDataRef> histogram_matching(const Array<TData>& data, const Array<TDataRef>& data_ref, size_t nbins = 10) {
    HistogramMatching<TData, TDataRef, TFloat> hm{data, data_ref, nbins};
    Array<TData> result{data.shape()};
    for (size_t i = 0; i < data.length(); ++i) {
        result(i) = hm(data(i));
    }
    return result;
}

}
