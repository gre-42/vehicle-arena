// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Stats/Random_Number_Generators.hpp>
#include <complex>

namespace Mlib {

/*
 * Uniformly random array
 */
template <class TFloat>
Array<std::complex<TFloat>> uniform_random_complex_array(const ArrayShape& shape, unsigned int seed) {
    auto ar = uniform_random_array<std::complex<TFloat>>(shape, seed);
    auto ai = uniform_random_array<std::complex<TFloat>>(shape, seed + 1);
    return ar + std::complex<TFloat>(0, 1) * ai;
}

template <class TDerived, class TData>
void randomize_array_uniform(BaseDenseArray<TDerived, TData>& a, unsigned int seed) {
    UniformRandomNumberGenerator<TData> r{seed};
    for (TData& v : a->flat_iterable()) {
        v = r();
    }
}

template <class TData>
Array<TData> uniform_random_array(const ArrayShape& shape, unsigned int seed) {
    Array<TData> a(shape);
    randomize_array_uniform(a, seed);
    return a;
}

/*
 * Normal random array
 */
template <class TFloat>
Array<std::complex<TFloat>> normal_random_complex_array(const ArrayShape& shape, unsigned int seed) {
    auto ar = normal_random_array<std::complex<TFloat>>(shape, seed);
    auto ai = normal_random_array<std::complex<TFloat>>(shape, seed + 1);
    return ar + std::complex<TFloat>(0, 1) * ai;
}

template <class TDerived, class TData>
void randomize_array_normal(BaseDenseArray<TDerived, TData>& a, unsigned int seed) {
    NormalRandomNumberGenerator<TData> r{seed};
    for (TData& v : a->flat_iterable()) {
        v = r();
    }
}

template <class TData>
Array<TData> normal_random_array(const ArrayShape& shape, unsigned int seed) {
    Array<TData> a(shape);
    randomize_array_uniform(a, seed);
    return a;
}

}
