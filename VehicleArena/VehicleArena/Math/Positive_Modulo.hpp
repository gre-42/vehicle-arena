// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cmath>
#include <concepts>

namespace VA {

// From: https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
template <std::floating_point TData>
TData positive_modulo(TData i, TData n) {
    return std::fmod(std::fmod(i, n) + n, n);
}

// From: https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
inline int positive_modulo(int i, int n) {
    return (i % n + n) % n;
}

}
