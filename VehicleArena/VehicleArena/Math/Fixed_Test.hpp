// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Math.hpp>
#include <stdexcept>

namespace VA {

template <class TData, size_t tshape0, size_t ...tshape>
void assert_allclose(const FixedArray<TData, tshape0, tshape...>& a, const FixedArray<TData, tshape0, tshape...>& b, typename FloatType<TData>::value_type atol = (typename FloatType<TData>::value_type)1e-6) {
    for (size_t i = 0; i < tshape0; ++i) {
        assert_allclose(a[i], b[i], atol);
    }
}

template <class TData>
void assert_allclose(const FixedArray<TData>& a, const FixedArray<TData>& b, typename FloatType<TData>::value_type atol = (typename FloatType<TData>::value_type)1e-6) {
    if (!isclose(a(), b(), atol)) {
        std::stringstream sstr;
        sstr << "Numbers not close (atol=" << atol << "):" << a() << ", " << b();
        throw std::runtime_error(sstr.str());
    }
}

template <class TData, size_t tshape0, size_t... tshape>
void assert_allequal(const FixedArray<TData, tshape0, tshape...>& a, const FixedArray<TData, tshape0, tshape...>& b) {
    for (size_t i = 0; i < tshape0; ++i) {
        assert_allequal(a[i], b[i]);
    }
}

template <class TData>
void assert_allequal(const FixedArray<TData>& a, const FixedArray<TData>& b) {
    if (!(a() == b()) && !(scalar_isnan(a()) && scalar_isnan(b()))) {
        throw std::runtime_error("Numbers not identical: " + std::to_string(a()) + ", " + std::to_string(b()));
    }
}

}
