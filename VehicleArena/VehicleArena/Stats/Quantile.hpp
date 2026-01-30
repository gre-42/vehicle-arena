// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Stats/Sort.hpp>
#include <stdexcept>

namespace VA {

template <class TDataX>
class Quantiles {
public:
    explicit Quantiles(const Array<TDataX>& x)
    : sx_(sorted(x))
    {
        if (x.length() == 0) {
            throw std::runtime_error("Cannot compute quantiles for an empty array");
        }
    }
    template <class TDataQ>
    const TDataX& operator () (const TDataQ& q, const TDataQ& offset = TDataQ(0.5)) {
        assert(q >= 0);
        assert(q <= 1);
        return sx_(size_t(q * TDataQ(sx_.length() - 1) + offset));
    }
private:
    Array<TDataX> sx_;
};

template <class TDataX, class TDataQ>
Array<TDataX> quantiles(const Array<TDataX>& x, const Array<TDataQ>& q) {
    Quantiles<TDataX> sx{x};
    Array<TDataX> res(q.shape());
    for (size_t i = 0; i < q.length(); ++i) {
        res(i) = sx(q(i));
    }
    return res;
}

template <class TDataX, class TDataQ>
TDataX quantile(const Array<TDataX>& x, const TDataQ& q) {
    return quantiles(x, Array<TDataQ>{q})(0);
}

template <class TDataX, class TDataQ>
Array<TDataX> nanquantiles(const Array<TDataX>& x, const Array<TDataQ>& q) {
    return quantiles(x[!VA::isnan(x)], q);
}

template <class TDataX, class TDataQ>
TDataX nanquantile(const Array<TDataX>& x, const TDataQ& q) {
    return quantile(x[!VA::isnan(x)], q);
}

}
