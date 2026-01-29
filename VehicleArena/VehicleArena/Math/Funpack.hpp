// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array_Forward.hpp>
#include <VehicleArena/Math/Fixed_Point_Number.hpp>
#include <concepts>

namespace VA {

template <class TInt, std::intmax_t denominator>
constexpr inline auto funpack(const Array<FixedPointNumber<TInt, denominator>>& a) {
    using I = intermediate_float<TInt>;
    return a.template casted<I>();
}

template <std::floating_point TFloat, size_t... tshape>
constexpr inline const auto& funpack(const Array<TFloat>& a) {
    return a;
}

template <class TInt, std::intmax_t denominator, size_t... tshape>
constexpr inline auto funpack(const FixedArray<FixedPointNumber<TInt, denominator>, tshape...>& a) {
    using I = intermediate_float<TInt>;
    return a.template casted<I>();
}

template <std::floating_point TFloat, size_t... tshape>
constexpr inline const auto& funpack(const FixedArray<TFloat, tshape...>& a) {
    return a;
}

template <class TInt, std::intmax_t denominator>
constexpr inline auto funpack(const FixedPointNumber<TInt, denominator>& a) {
    using I = intermediate_float<TInt>;
    return (I)a;
}

template <std::floating_point F>
constexpr inline auto funpack(const F& f) {
    return f;
}

template <std::integral I>
constexpr inline auto funpack(const I& i) {
    return i;
}

template <class T>
struct funpacks {
    static const T& func();
    using value_type = decltype(funpack(func()));
};

template <class T>
using funpack_t = funpacks<T>::value_type;

}
