// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Lerp.hpp>
#include <VehicleArena/Math/Positive_Modulo.hpp>
#include <VehicleArena/Math/Time_Point_Series.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <stdexcept>

namespace VA {

template <class TPos, size_t tndim, size_t length>
class PositionSeries {
public:
    PositionSeries()
        : data_{ uninitialized }
    {}
    PositionSeries(
        const FixedArray<TPos, tndim>& position,
        std::chrono::steady_clock::time_point time)
        : PositionSeries()
    {
        append(position, time);
    }

    void clear() {
        static_assert(length > 0);
        times_.clear();
    }

    void append(const FixedArray<TPos, tndim>& position, std::chrono::steady_clock::time_point time) {
        static_assert(length > 0);
        times_.append(time);
        data_(times_.last()) = position;
    }

    FixedArray<TPos, tndim> get(std::chrono::steady_clock::time_point time) const {
        return get(times_.interpolator(time));
    }

    FixedArray<TPos, tndim> get(const Interpolator& interp) const {
        return lerp(data_(interp.i0), data_(interp.i1), (TPos)interp.alpha);
    }

private:
    FixedArray<FixedArray<TPos, tndim>, length> data_;
    TimePointSeries<length> times_;
};

}
