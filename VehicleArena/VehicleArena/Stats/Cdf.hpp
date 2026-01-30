// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Stats/Histogram.hpp>

namespace VA {

template <class TData, class TFloat=TData>
class Cdf {
public:
    Cdf(const Array<TData>& data, size_t nbins = 10)
    : hist_{data, nbins}
    {
        const Array<size_t>& hist = hist_.hist();
        cdf_.resize(hist.shape());
        TFloat cumsum = 0;
        for (size_t i = 0; i < hist.length(); ++i) {
            cumsum += TFloat(hist(i)) / TFloat(data.length());
            cdf_(i) = std::min((TFloat)1, cumsum);
        }
    }
    const TFloat& operator () (const TData& v, bool check_bounds = false) {
        return cdf_(hist_.bin_id(v, check_bounds));
    }
    const Array<TFloat>& cdf() {
        return cdf_;
    }
    Array<TData> bins() {
        return hist_.bins();
    }
private:
    Histogram<TData> hist_;
    Array<TFloat> cdf_;
};

template <class TData, class TFloat=TData>
void cdf(const Array<TData>& data, Array<TFloat>& cdf, Array<TData>& bins, size_t nbins = 10) {
    Cdf ccdf{data, nbins};
    cdf = ccdf.cdf();
    bins = ccdf.bins();
}

}
