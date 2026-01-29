// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Ols.hpp>

namespace VA {

template <class TData>
class NaiveBayesRegression {
private:
    Array<Ols<TData>> ols_;
public:
    void train(const Array<TData>& predictors, const Array<TData>& responses)
    {
        assert(predictors.ndim() == 2);
        assert(responses.ndim() == 1);
        assert(predictors.shape(1) == responses.shape(0));
        ols_.resize(predictors.shape(0));
        for (size_t i=0; i<ols_.length(); i++) {
            ols_(i).train(predictors[i], responses, (TData)1e-6);
        }
    }
    TData predicted(const Array<TData>& predictors) const {
        assert(all(predictors.shape() == ols_.shape()));
        TData weighted_predicted = 0;
        TData sum_confidences = 0;
        for (size_t i=0; i<ols_.length(); i++) {
            const TData confidence = ols_(i).confidence(predictors(i));
            sum_confidences += confidence;
            weighted_predicted += confidence * ols_(i).predicted(predictors(i));
        }
        return weighted_predicted / sum_confidences;
    }
};

}