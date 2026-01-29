// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Pow.hpp>
#include <VehicleArena/Type_Traits/Get_Scalar.hpp>

namespace VA {

// From: https://math.stackexchange.com/a/3253471/233679
// See also the comment from Dr. Timofey Prodanov,
// Dec 21, 2020 at 10:51
template <class X>
static X sigmoid(X x)
{
    auto c1 = scalar_type_t<X>(1);
    auto a = squared(c1 / x - c1);
    return c1 / (c1 + a);
}

// From: https://math.stackexchange.com/a/3253471/233679
// See also the comment from Dr. Timofey Prodanov,
// Dec 21, 2020 at 10:51
template <class X, class K>
static X sigmoid(const X& x, const K& k)
{
    auto c1 = scalar_type_t<X>(1);
    auto a = pow(c1 / x - c1, k);
    return c1 / (c1 + a);
}

// From: https://math.stackexchange.com/a/4798714/233679
template <class X, class T, class K>
static X sigmoid(const X& x, const T& t, const K& k)
{
    auto c1 = scalar_type_t<X>(1);
    auto a = pow(pow(x, std::log((T)2) / std::log(t)) - c1, k);
    return c1 / (c1 + a);
}


}
