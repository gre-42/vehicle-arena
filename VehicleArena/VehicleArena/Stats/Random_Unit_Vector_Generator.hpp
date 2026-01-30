// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Stats/Fast_Random_Number_Generators.hpp>
#include <cstdint>

namespace VA {

template <class TData, size_t tndim>
class RandomUnitVectorGenerator {
public:
    explicit RandomUnitVectorGenerator(unsigned int seed)
        : rng_{ seed }
    {}
    FixedArray<TData, tndim> operator () () {
        while (true) {
            FixedArray<TData, tndim> result = uninitialized;
            for (auto& v : result.flat_iterable()) {
                v = rng_();
            }
            auto len = std::sqrt(sum(squared(result)));
            if (len > 1e-5) {
                return result / len;
            }
        }
    }
    FixedArray<TData, tndim> surface(const FixedArray<TData, tndim>& surface_normal) {
        auto res = (*this)();
        auto h = dot0d(res, surface_normal);
        if (h < 0) {
            res -= 2 * h * surface_normal;
        }
        return res;
    }
    FixedArray<TData, tndim> optional_surface(const std::optional<FixedArray<TData, tndim>>& surface_normal) {
        return surface_normal.has_value()
            ? surface(*surface_normal)
            : (*this)();
    }
    void seed(unsigned int seed) {
        rng_.seed(seed);
    }
private:
    FastNormalRandomNumberGenerator<TData> rng_;
};

}
