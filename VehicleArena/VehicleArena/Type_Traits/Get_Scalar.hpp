// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Point_Number.hpp>
#include <type_traits>

namespace VA {

struct IsScalarHelper {
    static const bool value = true;
};

struct IsNotScalarHelper {
    static const bool value = false;
};

template <class T>
IsNotScalarHelper is_scalar_helper(const T&);

template <class TInt, std::intmax_t denominator>
IsScalarHelper is_scalar_helper(const FixedPointNumber<TInt, denominator>&);

template <class T>
IsScalarHelper is_scalar_helper(const T&) requires std::is_scalar_v<T>;

template <class T>
struct ScalarType;

template <class T, bool is_sclr>
struct ScalarTypeRecursion;

template <class T>
struct ScalarTypeRecursion<T, true> {
    using value_type = T;
};

template <class T>
struct ScalarTypeRecursion<T, false> {
    using value_type = T::value_type;
};

template <class T>
struct ScalarType {
    static const T& func();
    using value_type = ScalarTypeRecursion<T, decltype(is_scalar_helper(func()))::value>::value_type;
};

template <class T>
using scalar_type_t = typename ScalarType<T>::value_type;

}
