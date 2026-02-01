// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Math.hpp>

namespace VA {

template <class TData>
class LinearInterpolator {
public:
    template <typename... TDimensions>
    TData operator () (const Array<TData>& im, TDimensions... dim) {
        TData v0 = ((1 - a0) * im(dim..., r0) + a0 * im(dim..., r1));
        return v0;
    }
    size_t r0;
    size_t r1;
    TData a0;
};

template <class TData>
bool linear_interpolation(TData rf, size_t nrows, LinearInterpolator<TData>& li) {
    if (rf < 0) {
        return false;
    }
    li.r0 = size_t(rf);
    li.r1 = li.r0 + 1;
    if (li.r1 == nrows) {
        --li.r1;
        --li.r0;
    }
    if (li.r1 >= nrows) {
        return false;
    }
    li.a0 = rf - (TData)li.r0;
    return true;
}

template <class TData>
bool linear_grayscale_interpolation(TData rf, const Array<TData>& im, TData& intensity) {
    assert(im.ndim() == 1);
    LinearInterpolator<TData> li;
    if (linear_interpolation(rf, im.shape(0), li)) {
        intensity = li(im);
        return true;
    } else {
        return false;
    }
}

template <class TData>
class InterpolationDomain1D {
public:
    InterpolationDomain1D() {}
    InterpolationDomain1D(TData xmin, TData xmax)
        : xmin_{ xmin }
        , inv_length_{ 1 / (xmax - xmin) }
    {}
    TData operator () (TData x, size_t nrows) const {
        TData rf = (x - xmin_) * inv_length_ * TData(nrows - 1);
        return rf;
    }
private:
    TData xmin_;
    TData inv_length_;
};

template <class TData>
class LinearInterpolationDomain {
public:
    LinearInterpolationDomain() {}
    LinearInterpolationDomain(TData xmin, TData xmax, const Array<TData>& im)
        : domain_{ xmin, xmax }
        , im_(im)
    {}
    bool operator () (TData x, TData& intensity) const {
        auto rf = domain_(x, im_.length());
        return linear_grayscale_interpolation(rf, im_, intensity);
    }
private:
    InterpolationDomain1D<TData> domain_;
    Array<TData> im_;
};

}
