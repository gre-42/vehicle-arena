// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Type_Traits/Any_Signed.hpp>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace VA {

template <class T, int64_t num>
constexpr bool fits() {
    return (
        num >= std::numeric_limits<T>::lowest() &&
        num <= std::numeric_limits<T>::max());
}

template <class T, auto num>
constexpr auto get_operand() {
    if constexpr (is_any_signed_v<T>) {
        if constexpr (fits<int8_t, num>()) {
            return (int8_t)num;
        } else if constexpr (fits<int16_t, num>()) {
            return (int16_t)num;
        } else if constexpr (fits<int32_t, num>()) {
            return (int32_t)num;
        } else if constexpr (fits<int64_t, num>()) {
            return (int64_t)num;
        }
    } else {
        if constexpr (fits<uint8_t, num>()) {
            return (uint8_t)num;
        } else if constexpr (fits<uint16_t, num>()) {
            return (uint16_t)num;
        } else if constexpr (fits<uint32_t, num>()) {
            return (uint32_t)num;
        } else if constexpr (fits<uint64_t, num>()) {
            return (uint64_t)num;
        }
    }
}

template <class T, auto num>
constexpr static const auto operand = get_operand<T, num>();

}
