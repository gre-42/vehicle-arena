// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Cholesky.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Transformation/Diagonal_Scale_Matrix.hpp>

namespace VA {

template <class TData, size_t tsize>
std::optional<FixedArray<TData, tsize, tsize>> inv_preconditioned_rc(const FixedArray<TData, tsize, tsize>& a) {
	auto sr = (TData)1 / sqrt(sum<0>(squared(a)));
	auto a_sr = dot2d(a, DiagonalScaleMatrix{ sr });
	auto sc = (TData)1 / sqrt(sum<1>(squared(a_sr)));
	auto sc_a_sr = dot2d(DiagonalScaleMatrix{ sc }, a_sr);
	auto i = inv(sc_a_sr);
	if (!i.has_value()) {
		return std::nullopt;
	}
	return dot2d(dot2d(DiagonalScaleMatrix{ sr }, *i), DiagonalScaleMatrix{ sc });
}

template <class TData, size_t tsize>
std::optional<FixedArray<TData, tsize, tsize>> inv_preconditioned_cr(const FixedArray<TData, tsize, tsize>& a) {
	auto sc = (TData)1 / sqrt(sum<1>(squared(a)));
	auto sc_a = dot2d(DiagonalScaleMatrix{ sc }, a);
	auto sr = (TData)1 / sqrt(sum<0>(squared(sc_a)));
	auto sc_a_sr = dot2d(sc_a, DiagonalScaleMatrix{ sr });
	auto i = inv(sc_a_sr);
	if (!i.has_value()) {
		return std::nullopt;
	}
	return dot2d(dot2d(DiagonalScaleMatrix{ sr }, *i), DiagonalScaleMatrix{ sc });
}

}
