// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Is_Power_Of_Two.hpp>
#include <bit>
#include <stdexcept>

namespace VA {

template <class T>
class PowerOfTwoDivider {
public:
    explicit PowerOfTwoDivider(T n)
        : n_{ n }
    {
        if (!is_power_of_two(n)) {
            throw std::runtime_error("PowerOfTwoDivider: Number is not a power of 2");
        }
    }
    T greatest_divider(const T n2) const {
        auto res = std::max<T>(1, std::bit_ceil(n2));
        if (res > n_) {
            throw std::runtime_error("PowerOfTwoDivider: Number is too large");
        }
        return res;
    }
private:
    T n_;
};

}
