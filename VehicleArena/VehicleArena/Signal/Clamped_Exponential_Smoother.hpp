// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Interval.hpp>
#include <VehicleArena/Math/Lerp.hpp>
#include <VehicleArena/Uninitialized.hpp>
#include <algorithm>
#include <cmath>
#include <optional>

namespace VA {

template <class TData, class TFloat=TData>
class ClampedExponentialSmoother {
public:
    explicit ClampedExponentialSmoother(const TFloat& alpha, const Interval<TData>& interval)
        : alpha_{ alpha }
        , interval_{ interval }
    {}
    ClampedExponentialSmoother(const TFloat& alpha, const Interval<TData>& interval, Uninitialized)
        : alpha_{ alpha }
        , interval_{ interval }
    {}
    template <class TX0>
    ClampedExponentialSmoother(const TFloat& alpha, const Interval<TData>& interval, const TX0& x0)
        : alpha_{ alpha }
        , interval_{ interval }
        , s_{ x0 }
    {}
    const TData& operator () (const TData& x) {
        if (s_.has_value()) {
            *s_ += std::clamp(lerp(*s_, x, alpha_) - *s_, interval_.min, interval_.max);
        } else {
            s_ = x;
        }
        return *s_;
    }
    ClampedExponentialSmoother changed_time_step(const TFloat& from, const TFloat& to) const {
        auto f = to / from;
        return { std::pow(alpha_, 1 / f), {interval_.min * f, interval_.max * f}, s_ };
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
    Interval<TData> interval_; 
    std::optional<TData> s_;
};

}
