// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Initialization/Uninitialized.hpp>
#include <VehicleArena/Math/Lerp.hpp>
#include <cmath>
#include <optional>

namespace VA {

template <class TData, class TFloat=TData>
class ExponentialSmoother {
public:
    explicit ExponentialSmoother(const TFloat& alpha)
        : alpha_{ alpha }
    {}
    ExponentialSmoother(const TFloat& alpha, Uninitialized)
        : alpha_{ alpha }
    {}
    ExponentialSmoother(const TFloat& alpha, const TData& x0)
        : alpha_{ alpha }
        , s_{ x0 }
    {}
    template <class TX0>
    ExponentialSmoother(const TFloat& alpha, const TX0& x0, const TFloat& dt)
        : alpha_{ std::pow(alpha, dt) }
        , s_{ x0 }
    {}
    const TData& operator () (const TData& x) {
        if (s_.has_value()) {
            *s_ = lerp(*s_, x, alpha_);
        } else {
            s_ = x;
        }
        return *s_;
    }
    const TData& operator () (const TData& x, const TFloat& dt) {
        if (s_.has_value()) {
            *s_ = lerp(*s_, x, std::pow(alpha_, 1 / dt));
        } else {
            s_ = x;
        }
        return *s_;
    }
    ExponentialSmoother changed_time_step(const TFloat& from, const TFloat& to) const {
        auto f = from / to;
        return { alpha_, s_, f };
    }
    const std::optional<TData>& xhat() const {
        return s_;
    }
    void set(const TData& xhat) {
        s_ = xhat;
    }
    void reset() {
        s_.reset();
    }
private:
    TFloat alpha_;
    std::optional<TData> s_;
};

}
