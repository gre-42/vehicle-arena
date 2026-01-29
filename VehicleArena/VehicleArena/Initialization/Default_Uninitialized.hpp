// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Initialization/Uninitialized.hpp>
#include <concepts>
#include <type_traits>
#include <utility>

namespace VA {

template <class T>
class DefaultUnitialized: public T {
public:
    DefaultUnitialized()
        : T(uninitialized)
    {}
    template <class... Args>
    explicit DefaultUnitialized(Args&&... v)
        : T(std::forward<Args>(v)...)
    {}
    DefaultUnitialized& operator = (const T& other) {
        T& v = *this;
        v = other;
        return *this;
    }
    DefaultUnitialized& operator = (T&& other) {
        T& v = *this;
        v = std::move(other);
        return *this;
    }
    T& base() {
        return *this;
    }
    const T& base() const {
        return *this;
    }
};

// template <class T>
//     requires ( std::is_constructible_v<T, Uninitialized> && std::is_convertible_v<T, Uninitialized> )
// static DefaultUnitialized<T> default_uninitialized_func();
// 
// template <class T>
//     requires ( !std::is_constructible_v<T, Uninitialized> || !std::is_convertible_v<T, Uninitialized> )
// static T default_uninitialized_func();
// 
// template <class T>
// using default_uninitialized_t = decltype(default_uninitialized_func<T>());

template <class T>
using default_uninitialized_t = std::conditional<
    std::is_default_constructible_v<T>,
    T,
    DefaultUnitialized<T>>::type;

// template <class T>
// using default_uninitialized_t = std::conditional<
//     std::is_constructible_v<T, Uninitialized>,
//     DefaultUnitialized<T>,
//     T>::type;

}
