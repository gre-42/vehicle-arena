// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Initialization/Default_Uninitialized.hpp>
#include <VehicleArena/Initialization/Uninitialized.hpp>
#include <VehicleArena/Math/Abs.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Math/Max.hpp>
#include <VehicleArena/Math/Sqrt.hpp>
#include <VehicleArena/Memory/Integral_To_Float.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>
#include <stdexcept>

#ifdef __GNUC__
    #pragma GCC push_options
    #pragma GCC optimize ("O3")
#endif

namespace VA {

template <class TDir, class TPos, size_t tndim>
class PlaneNd;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;

template <class TPos, size_t tndim>
class BoundingSphere {
public:
    BoundingSphere(Uninitialized)
        : center{ uninitialized }
    {}
    BoundingSphere(
        const FixedArray<TPos, tndim>& center,
        const TPos& radius)
        : center{ center }
        , radius{ radius }
    {}
    explicit BoundingSphere(const FixedArray<TPos, 2, tndim>& line)
        : BoundingSphere{ from_iterator(line.row_begin(), line.row_end()) }
    {}
    template <size_t tnpoints>
    explicit BoundingSphere(const FixedArray<TPos, tnpoints, tndim>& points)
        : BoundingSphere{ from_iterator(points.row_begin(), points.row_end()) }
    {}
    template <class TIterable>
    static BoundingSphere from_center_and_iterator(
        const FixedArray<TPos, tndim>& center,
        const TIterable& iterator_begin,
        const TIterable& iterator_end)
    {
        auto radius2 = funpack_t<TPos>(0);
        for (auto it = iterator_begin; it != iterator_end; ++it) {
            radius2 = max(radius2, sum(squared(*it - center)));
        }
        return BoundingSphere(center, (TPos)std::sqrt(radius2));
    }
    template <class TIterable>
    static BoundingSphere from_iterator(
        const TIterable& iterator_begin,
        const TIterable& iterator_end)
    {
        size_t nelements = 0;
        using I = funpack_t<TPos>;
        FixedArray<I, tndim> center((I)0);
        for (auto it = iterator_begin; it != iterator_end; ++it) {
            center += it->template casted<I>();
            ++nelements;
        }
        if (nelements == 0) {
            throw std::runtime_error("Bounding sphere received no elements");
        }
        center /= integral_to_float<I>(nelements);
        return from_center_and_iterator(center.template casted<TPos>(), iterator_begin, iterator_end);
    }
    bool contains(const FixedArray<TPos, tndim>& other, const TPos& tolerance = TPos(0.f)) const {
        return sum(squared(other - center)) <= squared(radius + tolerance);
    }
    bool intersects(const BoundingSphere& other, const TPos& tolerance = TPos(0.f)) const {
        return sum(squared(other.center - center)) <= squared(other.radius + radius + tolerance);
    }
    template <class TDir>
    bool intersects(const PlaneNd<TDir, TPos, tndim>& plane) const {
        using fa = funpack_t<TPos>;
        auto dist = (TPos)dot0d(plane.normal.template casted<fa>(), center.template casted<fa>()) + plane.intercept;
        return abs(dist) <= radius;
    }
    void extend(const BoundingSphere& other) {
        radius = std::max(radius, (TPos)std::sqrt(sum(squared(other.center - center))) + other.radius);
    }
    template <class TTDir, class TTPos>
    BoundingSphere<TPos, tndim> transformed(const TransformationMatrix<TTDir, TTPos, tndim>& transformation_matrix) const {
        return BoundingSphere<TPos, tndim>{
            transformation_matrix.transform(center.template casted<TTPos>()).template casted<TPos>(),
            radius};
    }
    template <class TTDir, class TTPos>
    BoundingSphere<TPos, tndim> itransformed(const TransformationMatrix<TTDir, TTPos, tndim>& transformation_matrix) const {
        return BoundingSphere<TPos, tndim>{
            transformation_matrix.itransform(center.template casted<TTPos>()).template casted<TPos>(),
            radius};
    }
    template <class TResultData>
    BoundingSphere<TResultData, tndim> casted() const {
        return BoundingSphere<TResultData, tndim>(
            center.template casted<TResultData>(),
            (TResultData)radius);
    }
    bool operator == (const BoundingSphere& other) const {
        return all(center == other.center) &&
               (radius == other.radius);
    }
    FixedArray<TPos, tndim> center;
    TPos radius;
};

template <class TPos, size_t tndim>
BoundingSphere<TPos, tndim> operator + (
    const BoundingSphere<TPos, tndim>& b,
    const FixedArray<TPos, tndim>& x)
{
    return { b.center + x, b.radius };
}

template <class TPos, size_t tndim>
using UBoundingSphere = DefaultUnitialized<BoundingSphere<TPos, tndim>>;

}

#ifdef __GNUC__
    #pragma GCC pop_options
#endif
