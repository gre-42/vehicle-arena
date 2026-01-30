// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Consteval_Workaround.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Memory/Integral_To_Float.hpp>
#include <VehicleArena/Testing/Assert.hpp>
#include <VehicleArena/Type_Traits/Get_Scalar.hpp>
#include <VehicleArena/Type_Traits/Operand.hpp>

namespace VA {

template <class TData>
TData mean(const Array<TData>& a) {
    assert_true(a.nelements() > 0);
    return sum(a) / integral_to_float<scalar_type_t<TData>>(a.nelements());
}

template <class TData, size_t... tsize>
auto mean(const FixedArray<TData, tsize...>& a) {
    // Produces "function parameter 'a' with unknown value cannot be used in a constant expression".
    // constexpr size_t n = CW::nelements(a);
    constexpr size_t n = FixedArray<TData, tsize...>::nelements();
    static_assert(n > 0);
    return sum(a) / operand<scalar_type_t<TData>, n>;
}

template <size_t axis, class TData, size_t... tsize>
auto mean(const FixedArray<TData, tsize...>& a) {
    // Produces "function parameter 'a' with unknown value cannot be used in a constant expression".
    // constexpr size_t n = CW::static_shape<axis>(a);
    constexpr size_t n = FixedArray<TData, tsize...>::template static_shape<axis>();
    static_assert(n > 0);
    return sum<axis>(a) / operand<TData, n>;
}

template <class TData>
Array<TData> mean(const Array<TData>& a, size_t axis) {
    assert(a.shape(axis) > 0);
    return sum(a, axis) / a.shape(axis);
}

template <class TDerived, class TData>
TData nanmean(const BaseDenseArray<TDerived, TData>& a) {
    return mean((*a)[!VA::isnan(a)]);
}

}
