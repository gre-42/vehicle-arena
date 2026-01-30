// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Stats/Linspace.hpp>
#include <VehicleArena/Stats/Mean.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>

namespace VA {

template <class TData>
class Histogram {
public:
    Histogram(const Array<TData>& data, size_t nbins = 10) {
        assert(nbins > 1);
        Array<TData> fdata = data[VA::isfinite(data)];
        mi_ = min(fdata);
        ma_ = max(fdata);
        // TData h = (ma - mi) / (nbins - 1);
        // Array<TData> boundaries = linspace(mi - h / 2, ma + h / 2, nbins + 1);
        hist_ = zeros<size_t>(ArrayShape{nbins});
        for (const TData& v : fdata.flat_iterable()) {
            // for (size_t j = 1; j < boundaries.length(); ++j) {
            //     if (const TData& v = fdata(i); v >= boundaries(j - 1) && v < boundaries(j)) {
            //         ++hist(j - 1);
            //     }
            // }
            ++hist_(bin_id(v));
        }
    }
    const Array<size_t>& hist() {
        return hist_;
    }
    Array<TData> bins() {
        return linspace(mi_, ma_, hist_.length());
    }
    size_t bin_id(const TData& data, bool check_bounds = false) {
        if (check_bounds) {
            if (data < mi_) {
                return 0;
            }
            if (data > ma_) {
                return hist_.length() - 1;
            }
        }
        return (size_t)((TData(hist_.length() - 1) * (data - mi_)) / (ma_ - mi_));
    }
private:
    Array<size_t> hist_;
    TData mi_;
    TData ma_;
};

template <class TData>
void histogram(const Array<TData>& data, Array<size_t>& hist, Array<TData>& bins, size_t nbins = 10) {
    Histogram hm{data, nbins};
    hist = hm.hist();
    bins = hm.bins();
}

}
