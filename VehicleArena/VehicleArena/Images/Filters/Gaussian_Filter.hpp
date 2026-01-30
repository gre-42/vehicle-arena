// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Images/Filters/Gaussian_Kernel.hpp>
#include <VehicleArena/Images/Filters/Lowpass_Filtering.hpp>
#include <VehicleArena/Images/Filters/Polynomial_Contrast.hpp>
#include <VehicleArena/Stats/Linspace.hpp>
#include <stdexcept>

namespace VA {

template <class TData, class TSigma>
Array<TData> gaussian_filter_1d_NWE(
    const Array<TData>& image,
    const TSigma& sigma,
    size_t axis,
    const TData& boundary_value,
    const TSigma& truncate = 4,
    FilterExtension fc = FilterExtension::NWE,
    size_t poly_degree = 0)
{
    if (sigma == 0) {
        return image.copy();
    }
    auto coeffs = gaussian_kernel(sigma, truncate);
    poly_degree = (poly_degree / 2) * 2;
    if (poly_degree != 0) {
        Array<TSigma> contrast = zeros<TSigma>(ArrayShape{ 1 + poly_degree });
        contrast(0) = 1;
        Array<Array<TSigma>> A{ ArrayShape{ 1 }};
        A[0] = linspace<TSigma>(-1, 1, coeffs.length());
        coeffs = polynomial_contrast(A, coeffs, contrast, poly_degree);
    }
    return lowpass_filter_1d_NWE(image, coeffs, boundary_value, axis, fc);
}

template <class TData, class TSigma>
Array<TData> gaussian_filter_NWE(
    const Array<TData>& image,
    const TSigma& sigma,
    const TData& boundary_value,
    const TSigma& truncate = 4,
    FilterExtension fc = FilterExtension::NWE,
    size_t poly_degree = 0)
{
    if (image.ndim() == 0) {
        return image.copy();
    }
    Array<TData> result;
    for (size_t axis = 0; axis < image.ndim(); ++axis) {
        result.move() = gaussian_filter_1d_NWE(axis == 0 ? image : result, sigma, axis, boundary_value, truncate, fc, poly_degree);
    }
    return result;
}

template <class TData>
Array<TData> multichannel_gaussian_filter_NWE(
    const Array<TData>& image,
    const TData& sigma,
    const TData& boundary_value,
    const TData& truncate = 4,
    FilterExtension fc = FilterExtension::NWE,
    size_t poly_degree = 0)
{
    if (image.ndim() == 0) {
        throw std::runtime_error("Image dimension must be > 0");
    }
    Array<TData> result{ image.shape() };
    for (size_t h = 0; h < image.shape(0); ++h) {
        result[h] = std::move(gaussian_filter_NWE(image[h], sigma, boundary_value, truncate, fc, poly_degree));
    }
    return result;
}

}
