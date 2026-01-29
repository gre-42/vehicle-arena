// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Array/Fixed_Array_Hash.hpp>
#include <VehicleArena/Default_Uninitialized.hpp>
#include <VehicleArena/Default_Uninitialized_Element.hpp>
#include <VehicleArena/Hash.hpp>
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Std_Hash.hpp>
#include <compare>
#include <concepts>

namespace VA {

template <class TData, size_t... tshape>
class OrderableFixedArray;

template <class TData, size_t tshape0, size_t... tshape>
class OrderableFixedArray<TData, tshape0, tshape...>: public FixedArray<TData, tshape0, tshape...> {
public:
    using Base = FixedArray<TData, tshape0, tshape...>;
    using value_type = Base::value_type;
    using initializer_type = Base::initializer_type;

    OrderableFixedArray(NanInitialized) {
        *this = TData(nan_initialized);
    }
    OrderableFixedArray(Uninitialized)
        : Base{ uninitialized }
    {}
    template<std::convertible_to<TData> Value>
    explicit OrderableFixedArray(const Value& value)
        : Base{ value }
    {}
    template<std::convertible_to<initializer_type>... Values>
        requires (sizeof...(Values) == tshape0)
    OrderableFixedArray(const Values&... values)
        : Base{ values... }
    {}
    explicit OrderableFixedArray(const Base& rhs)
        : Base{ rhs }
    {}
    OrderableFixedArray& operator = (const TData& rhs) {
        Base& f = *this;
        f = rhs;
        return *this;
    }
    OrderableFixedArray& operator = (const Base& rhs) {
        Base& f = *this;
        f = rhs;
        return *this;
    }
    bool operator < (const OrderableFixedArray& rhs) const {
        return this->less_than(rhs);
    }
    bool operator == (const OrderableFixedArray& rhs) const {
        const Base& lhs = *this;
        const Base& rhs2 = rhs;
        return all(lhs == rhs2);
    }
    bool operator != (const OrderableFixedArray& rhs) const {
        return !(*this == rhs);
    }
    bool operator > (const OrderableFixedArray& rhs) const {
        //return (*this != rhs) && (!(*this < rhs));
        return static_cast<const OrderableFixedArray&>(rhs) < *this;
    }
    std::strong_ordering operator <=> (const OrderableFixedArray& other) const {
        if (*this < other) {
            return std::strong_ordering::less;
        }
        if (*this == other) {
            return std::strong_ordering::equal;
        }
        return std::strong_ordering::greater;
    }
    bool all_equal(const TData& d) const {
        const Base& a = *this;
        return all(a == d);
    }
};

template <class TData, size_t... tshape>
OrderableFixedArray<TData, tshape...> make_orderable(const FixedArray<TData, tshape...>& a) {
    return OrderableFixedArray<TData, tshape...>{ a };
}

template <class TData, size_t... tshape>
using UOrderableFixedArray = DefaultUnitialized<OrderableFixedArray<TData, tshape...>>;

template <class TData, size_t... tshape>
using EOrderableFixedArray = DefaultUnitializedElement<OrderableFixedArray<TData, tshape...>>;

}

template <class TData, size_t... tshape>
struct std::hash<VA::OrderableFixedArray<TData, tshape...>>
{
    std::size_t operator() (const VA::OrderableFixedArray<TData, tshape...>& a) const {
        return VA::fixed_array_hash(a);
    }
};
