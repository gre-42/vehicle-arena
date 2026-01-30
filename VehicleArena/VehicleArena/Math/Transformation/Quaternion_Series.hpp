// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Positive_Modulo.hpp>
#include <VehicleArena/Math/Time_Point_Series.hpp>
#include <VehicleArena/Math/Transformation/Quaternion.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <stdexcept>

namespace VA {

template <class TDir, class TPos, size_t length>
class QuaternionSeries {
public:
    QuaternionSeries()
        : data_{ uninitialized }
    {}
    QuaternionSeries(
        const OffsetAndQuaternion<TDir, TPos>& qo,
        std::chrono::steady_clock::time_point time)
        : QuaternionSeries()
    {
        append(qo, time);
    }

    void clear() {
        static_assert(length > 0);
        times_.clear();
    }

    bool empty() const {
        return times_.empty();
    }

    void append(const OffsetAndQuaternion<TDir, TPos>& qo, std::chrono::steady_clock::time_point time) {
        static_assert(length > 0);
        times_.append(time);
        data_(times_.last()) = qo;
    }

    OffsetAndQuaternion<TDir, TPos> get(std::chrono::steady_clock::time_point time) const {
        return get(times_.interpolator(time));
    }

    OffsetAndQuaternion<TDir, TPos> get(const Interpolator& interp) const {
        return data_(interp.i0).slerp(data_(interp.i1), interp.alpha);
    }

    const OffsetAndQuaternion<TDir, TPos>& newest() const {
        return data_(times_.last());
    }

    std::chrono::steady_clock::time_point newest_time() const {
        return times_.newest_time();
    }

private:
    FixedArray<OffsetAndQuaternion<TDir, TPos>, length> data_;
    TimePointSeries<length> times_;
};

}
