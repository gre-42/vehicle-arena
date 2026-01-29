// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <limits>   

namespace VA {

namespace Detail {

template <class T>
T constexpr sqrt_newton_raphson(T x, T curr, T prev) {
    return curr == prev
        ? curr
        : sqrt_newton_raphson<T>(x, T(0.5) * (curr + x / curr), curr);
}

}

// Constexpr version of the square root
// Return value:
//   - For a finite and non-negative value of "x", returns an approximation for the square root of "x"
//   - Otherwise, returns NaN
// From: https://stackoverflow.com/a/34134071/2292832
// 
template <class T>
T constexpr sqrt_constexpr(T x)
{
    return x >= 0 && x < std::numeric_limits<T>::infinity()
        ? Detail::sqrt_newton_raphson<T>(x, x, 0)
        : std::numeric_limits<T>::quiet_NaN();
}

}
