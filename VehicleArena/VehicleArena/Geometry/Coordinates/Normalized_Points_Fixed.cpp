// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Normalized_Points_Fixed.hpp"
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>
#include <VehicleArena/Stats/Quantile.hpp>

using namespace VA;

template <class TData>
NormalizedPointsFixed<TData>::NormalizedPointsFixed(ScaleMode scale_mode, OffsetMode offset_mode)
: scale_mode_{scale_mode},
  offset_mode_{offset_mode}
{}

template <class TData>
void NormalizedPointsFixed<TData>::add_point(const FixedArray<TData, 2>& y)
{
    min_ = minimum(y, min_);
    max_ = maximum(y, max_);
}

template <class TData>
void NormalizedPointsFixed<TData>::set_min(const FixedArray<TData, 2>& p) {
    min_ = p;
}

template <class TData>
void NormalizedPointsFixed<TData>::set_max(const FixedArray<TData, 2>& p) {
    max_ = p;
}

template <class TData>
TransformationMatrix<TData, TData, 2> NormalizedPointsFixed<TData>::normalization_matrix() const {
    // dx: difference
    // sx: sum
    FixedArray<TData, 2> d = (offset_mode_ == OffsetMode::CENTERED) ? (max_ - min_) / TData(2) : (max_ - min_);
    FixedArray<TData, 2> s = (offset_mode_ == OffsetMode::CENTERED) ? (min_ + max_) / TData(2) : min_;
    if (scale_mode_ == ScaleMode::PRESERVE_ASPECT_RATIO) {
        d = std::max(d(0), d(1));
    }
    if (scale_mode_ == ScaleMode::NONE) {
        d = 1;
    }
    // (p - sx) / dx = p/dx - sx/dx
    // (p - sy) / dy = p/dy - sy/dy
    return TransformationMatrix<TData, TData, 2>{
        FixedArray<TData, 2, 2>::init(
            1 / d(0), (TData)0,
            TData(0), 1 / d(1)),
        FixedArray<TData, 2>::init(
            -s(0) / d(0),
            -s(1) / d(1))};
}

template <class TData>
NormalizedPointsFixed<TData> NormalizedPointsFixed<TData>::chained(ScaleMode scale_mode, OffsetMode offset_mode) const {
    NormalizedPointsFixed result{scale_mode, offset_mode};
    auto n = normalization_matrix();
    result.min_ = n.transform(min_);
    result.max_ = n.transform(max_);
    return result;
}

template <class TData>
const FixedArray<TData, 2>& NormalizedPointsFixed<TData>::min() const {
    return min_;
}

template <class TData>
const FixedArray<TData, 2>& NormalizedPointsFixed<TData>::max() const {
    return max_;
}

template class VA::NormalizedPointsFixed<float>;
template class VA::NormalizedPointsFixed<double>;
