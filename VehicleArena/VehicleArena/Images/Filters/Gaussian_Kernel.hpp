// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Array.hpp>
#include <VehicleArena/Memory/Float_To_Integral.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>
#include <stdexcept>

namespace VA {

enum class ShapeType {
    HALF,
    FULL
};

enum class NormalizationType {
    SUM,
    MAX
};

template <class TSigma>
Array<TSigma> gaussian_kernel(
    const TSigma& sigma,
    const TSigma& truncate,
    ShapeType shape = ShapeType::FULL,
    NormalizationType normalization = NormalizationType::SUM)
{
    Array<TSigma> coeffs;
    if (shape == ShapeType::HALF) {
        coeffs = Array<TSigma>(ArrayShape{{1 + float_to_integral<size_t>(std::round(truncate * sigma))}});
        for (size_t i = 0; i < coeffs.length(); ++i) {
            coeffs(i) = std::exp(-squared(TSigma(i) / sigma) / 2);
        }
    } else if (shape == ShapeType::FULL) {
        coeffs = Array<TSigma>(ArrayShape{{1 + 2 * float_to_integral<size_t>(std::round(truncate * sigma))}});
        size_t cdist = coeffs.length() / 2;
        for (size_t i = cdist; i < coeffs.length(); ++i) {
            coeffs(i) = std::exp(-squared(TSigma(i - cdist) / sigma) / 2);
            coeffs(coeffs.length() - i - 1) = coeffs(i);
        }
    }
    if (!coeffs.initialized()) {
        throw std::runtime_error("Unknown kernel shape type");
    }
    switch (normalization) {
    case NormalizationType::SUM:
        coeffs /= sum(coeffs);
        return coeffs;
    case NormalizationType::MAX:
        coeffs /= max(coeffs);
        return coeffs;
    }
    throw std::runtime_error("Unknown normalization type");
}

}
