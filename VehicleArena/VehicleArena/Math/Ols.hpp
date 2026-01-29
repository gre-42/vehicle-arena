// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Stats/Mean.hpp>
#include <cmath>

namespace VA {

template <class TData>
class Ols {
    TData mean_r_;
    TData mean_p_;
    TData r_;
    TData std_p_;
    TData coeff_;
public:
    void train(const Array<TData>& predictors, const Array<TData>& responses, TData alpha = 0) {
        assert(predictors.ndim() == 1);
        assert(all(predictors.shape() == responses.shape()));
        mean_p_ = mean(predictors);
        mean_r_ = mean(responses);
        const Array<TData> pm = predictors - mean_p_;
        const Array<TData> rm = responses - mean_r_;
        std_p_ = std::sqrt(dot0d(pm, pm)) * (1 + alpha) + alpha;
        const TData std_r = std::sqrt(dot0d(rm, rm)) * (1 + alpha) + alpha;
        r_ = (dot0d(pm, rm)) / std_r / std_p_;
        r_ = std::max<TData>(std::min<TData>(r_, 1), -1);
        coeff_ = std_r * r_ / std_p_;
        // lerr() << pm << " - " << rm << " " << alpha;
        // lerr() << alpha << " " << mean_r_ << " " << std_r << " " << r_ << " " << std_p_ << " " << mean_p_;
    }
    TData predicted(const TData& predictor) const {
        return mean_r_ + coeff_ * (predictor - mean_p_);
    }
    TData confidence(const TData& predictor) const {
        const TData dist = squared((predictor - mean_p_) / std_p_);
        return 1 / (1 + dist) * (1 / (1 - std::abs(r_)) - 1);
    }
};

}
