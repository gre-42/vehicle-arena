// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Convex_Polygon.hpp>
#include <VehicleArena/Initialization/Uninitialized.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Stats/Clamped.hpp>
#include <VehicleArena/Stats/Min_Max.hpp>
#include <array>
#include <iosfwd>
#include <string>

#ifdef __GNUC__
    #pragma GCC push_options
    #pragma GCC optimize ("O3")
#endif

namespace VA {

template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TDir, class TPos>
class RaySegment3D;

template <class TData, size_t tndim>
class AxisAlignedBoundingBox {
    template <class TData2, size_t tndim2>
    friend class AxisAlignedBoundingBox; 
public:
    using Bound = FixedArray<TData, tndim>;

    AxisAlignedBoundingBox(Uninitialized)
        : min{ uninitialized }
        , max{ uninitialized }
    {}
    static AxisAlignedBoundingBox empty() {
        return AxisAlignedBoundingBox{
            fixed_full<TData, tndim>(std::numeric_limits<TData>::max()),
            fixed_full<TData, tndim>(std::numeric_limits<TData>::lowest())};
    }
    static AxisAlignedBoundingBox zero() {
        return AxisAlignedBoundingBox{
            fixed_zeros<TData, tndim>(),
            fixed_zeros<TData, tndim>()};
    }
    static AxisAlignedBoundingBox from_min_max(
        const FixedArray<TData, tndim>& min,
        const FixedArray<TData, tndim>& max) {
        return AxisAlignedBoundingBox{ min, max };
    }
    static AxisAlignedBoundingBox from_point(const FixedArray<TData, tndim>& point) {
        return AxisAlignedBoundingBox{ point, point };
    }
    static AxisAlignedBoundingBox from_center_and_radius(
        const FixedArray<TData, tndim>& center,
        const TData& radius)
    {
        return AxisAlignedBoundingBox{
            center - radius,
            center + radius };
    }
    template <size_t tnpoints>
    static AxisAlignedBoundingBox from_points(
        const FixedArray<TData, tnpoints, tndim>& points)
    {
        return from_iterator(points.row_begin(), points.row_end());
    }
    static AxisAlignedBoundingBox from_points(
        const FixedArray<TData, tndim>& a,
        const FixedArray<TData, tndim>& b)
    {
        return AxisAlignedBoundingBox{ minimum(a, b), maximum(a, b) };
    }
    template <class TIterator>
    static AxisAlignedBoundingBox from_iterator(
        const TIterator& iterator_begin,
        const TIterator& iterator_end)
    {
        auto result = empty();
        for (auto it = iterator_begin; it != iterator_end; ++it) {
            result.extend(*it);
        }
        return result;
    }
    bool intersects(const AxisAlignedBoundingBox& other) const {
        // return all(max >= other.min) && all(min <= other.max);
        return all_ge(max, other.min) && all_le(min, other.max);
        // for (size_t i = 0; i < tndim; ++i) {
        //     if (max(i) < other.min(i)) {
        //         return false;
        //     }
        // }
        // for (size_t i = 0; i < tndim; ++i) {
        //     if (min(i) > other.max(i)) {
        //         return false;
        //     }
        // }
        // return true;
    }
    bool contains(const AxisAlignedBoundingBox& other) const {
        // return all(max >= other.max) && all(min <= other.min);
        for (size_t i = 0; i < tndim; ++i) {
            if (max(i) < other.max(i)) {
                return false;
            }
            if (min(i) > other.min(i)) {
                return false;
            }
        }
        return true;
    }
    bool contains(const FixedArray<TData, tndim>& point) const {
        // return all(max >= point) && all(min <= point);
        for (size_t i = 0; i < tndim; ++i) {
            if (max(i) < point(i)) {
                return false;
            }
            if (min(i) > point(i)) {
                return false;
            }
        }
        return true;
    }
    FixedArray<TData, tndim> closest_point(const FixedArray<TData, tndim>& point) const {
        return clamped(point, min, max);
    }
    void extend(const AxisAlignedBoundingBox& other) {
        min = (Bound)minimum(min, other.min);
        max = (Bound)maximum(max, other.max);
    }
    void extend(const FixedArray<TData, tndim>& point) {
        min = (Bound)minimum(min, point);
        max = (Bound)maximum(max, point);
    }
    template <class TDir, class TPos>
    AxisAlignedBoundingBox<TPos, tndim> transformed(
        const TransformationMatrix<TDir, TPos, tndim>& transformation_matrix) const
    {
        auto result = empty();
        for_each_corner([&](const FixedArray<TData, tndim>& corner){
            result.extend(transformation_matrix.transform(corner));
            return true;
        });
        return result;
    }
    AxisAlignedBoundingBox translated(const FixedArray<TData, tndim>& translation) const
    {
        return AxisAlignedBoundingBox(min + translation, max + translation);
    }
    FixedArray<TData, tndim> center() const {
        return (min + max) / (uint16_t)2;
    }
    FixedArray<TData, tndim> size() const {
        return max - min;
    }
    void print(std::ostream& ostr, size_t rec = 0) const {
        std::string indent(rec, ' ');
        ostr << indent << "bounds " << min << " -- " << max;
    }
    template <class TOperation>
    bool for_each_corner(const TOperation& op) const {
        FixedArray<TData, tndim> corner = uninitialized;
        return for_each_corner<0>(op, corner);
    }
    template <class TOperation>
    bool for_each_edge(const TOperation& op) const {
        static_assert(tndim == 3);
        const auto edges = FixedArray<TData, 12, 2, 3>{
            // 0, 0, 0
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), min(1), min(2)},
                FixedArray<TData, 3>{min(0), min(1), max(2)}
            },
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), min(1), min(2)},
                FixedArray<TData, 3>{min(0), max(1), min(2)}
            },
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), min(1), min(2)}
            },
            // 0, 0, 1
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), min(1), max(2)},
                FixedArray<TData, 3>{min(0), max(1), max(2)}
            },
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), min(1), max(2)},
                FixedArray<TData, 3>{max(0), min(1), max(2)}
            },
            // 0, 1, 0
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), max(1), min(2)},
                FixedArray<TData, 3>{min(0), max(1), max(2)}
            },
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), max(1), min(2)},
                FixedArray<TData, 3>{max(0), max(1), min(2)}
            },
            // 0, 1, 1
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{min(0), max(1), max(2)},
                FixedArray<TData, 3>{max(0), max(1), max(2)}
            },
            // 1, 0, 0
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{max(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), min(1), max(2)}
            },
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{max(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), max(1), min(2)}
            },
            // 1, 0, 1
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{max(0), min(1), max(2)},
                FixedArray<TData, 3>{max(0), max(1), max(2)}
            },
            // 1, 1, 0
            FixedArray<TData, 2, 3>{
                FixedArray<TData, 3>{max(0), min(1), max(2)},
                FixedArray<TData, 3>{max(0), max(1), max(2)}
            }
        };
        for (const auto& e : edges.row_iterable()) {
            if (!op(e)) {
                return false;
            }
        }
        return true;
    }
    template <class TDir, class TOperation>
    bool for_each_ray(const TOperation& op) const {
        static_assert(tndim == 3);
        const auto rays = FixedArray<RaySegment3D<TDir, TData>, 12>{
            // 0, 0, 0
            RaySegment3D<TDir, TData>{ min, FixedArray<TDir, 3>{0.f, 0.f, 1.f}, (TDir)(max(2) - min(2)) },
            RaySegment3D<TDir, TData>{ min, FixedArray<TDir, 3>{0.f, 1.f, 0.f}, (TDir)(max(1) - min(1)) },
            RaySegment3D<TDir, TData>{ min, FixedArray<TDir, 3>{1.f, 0.f, 0.f}, (TDir)(max(0) - min(0)) },
            // 0, 0, 1
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{min(0), min(1), max(2)},
                FixedArray<TDir, 3>{0.f, 1.f, 0.f},
                (TDir)(max(1) - min(1))
            },
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{min(0), min(1), max(2)},
                FixedArray<TDir, 3>{1.f, 0.f, 0.f},
                (TDir)(max(0) - min(0))
            },
            // 0, 1, 0
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{min(0), max(1), min(2)},
                FixedArray<TDir, 3>{0.f, 0.f, 1.f},
                (TDir)(max(2) - min(2))
            },
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{min(0), max(1), min(2)},
                FixedArray<TDir, 3>{1.f, 0.f, 0.f},
                (TDir)(max(0) - min(0))
            },
            // 0, 1, 1
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{min(0), max(1), max(2)},
                FixedArray<TDir, 3>{1.f, 0.f, 0.f},
                (TDir)(max(0) - min(0))
            },
            // 1, 0, 0
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{max(0), min(1), min(2)},
                FixedArray<TDir, 3>{0.f, 0.f, 1.f},
                (TDir)(max(2) - min(2))
            },
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{max(0), min(1), min(2)},
                FixedArray<TDir, 3>{0.f, 1.f, 0.f},
                (TDir)(max(1) - min(1))
            },
            // 1, 0, 1
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{max(0), min(1), max(2)},
                FixedArray<TDir, 3>{0.f, 1.f, 0.f},
                (TDir)(max(1) - min(1))
            },
            // 1, 1, 0
            RaySegment3D<TDir, TData>{
                FixedArray<TData, 3>{max(0), min(1), max(2)},
                FixedArray<TDir, 3>{0.f, 1.f, 0.f},
                (TDir)(max(1) - min(1))
            }
        };
        for (const auto& ray : rays.flat_iterable()) {
            if (!op(ray)) {
                return false;
            }
        }
        return true;
    }
    template <class TDir, class TOperation>
    bool for_each_face(const TOperation& op) const {
        static_assert(tndim == 3);
        auto faces = std::array<ConvexPolygon3D<TDir, TData, 4>, 6>{
            // x-
            ConvexPolygon3D<TDir, TData, 4>{FixedArray<TData, 4, 3>{
                FixedArray<TData, 3>{min(0), min(1), min(2)},
                FixedArray<TData, 3>{min(0), max(1), min(2)},
                FixedArray<TData, 3>{min(0), max(1), max(2)},
                FixedArray<TData, 3>{min(0), min(1), max(2)}}},
            // x+
            ConvexPolygon3D<TDir, TData, 4>{FixedArray<TData, 4, 3>{
                FixedArray<TData, 3>{max(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), max(1), min(2)},
                FixedArray<TData, 3>{max(0), max(1), max(2)},
                FixedArray<TData, 3>{max(0), min(1), max(2)}}},
            // y-
            ConvexPolygon3D<TDir, TData, 4>{FixedArray<TData, 4, 3>{
                FixedArray<TData, 3>{min(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), min(1), max(2)},
                FixedArray<TData, 3>{min(0), min(1), max(2)}}},
            // y+
            ConvexPolygon3D<TDir, TData, 4>{FixedArray<TData, 4, 3>{
                FixedArray<TData, 3>{min(0), max(1), min(2)},
                FixedArray<TData, 3>{max(0), max(1), min(2)},
                FixedArray<TData, 3>{max(0), max(1), max(2)},
                FixedArray<TData, 3>{min(0), max(1), max(2)}}},
            // z-
            ConvexPolygon3D<TDir, TData, 4>{FixedArray<TData, 4, 3>{
                FixedArray<TData, 3>{min(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), min(1), min(2)},
                FixedArray<TData, 3>{max(0), max(1), min(2)},
                FixedArray<TData, 3>{min(0), max(1), min(2)}}},
            // z+
            ConvexPolygon3D<TDir, TData, 4>{FixedArray<TData, 4, 3>{
                FixedArray<TData, 3>{min(0), min(1), max(2)},
                FixedArray<TData, 3>{max(0), min(1), max(2)},
                FixedArray<TData, 3>{max(0), max(1), max(2)},
                FixedArray<TData, 3>{min(0), max(1), max(2)}}}
        };
        for (const auto& f : faces) {
            if (!op(f)) {
                return false;
            }
        }
        return true;
    }
    template <class TOperation>
    bool for_each_cell(const TOperation& op) const {
        static_assert(tndim == 3);
        for (auto i0 = min(0); i0 <= max(0); ++i0) {
            for (auto i1 = min(1); i1 <= max(1); ++i1) {
                for (auto i2 = min(2); i2 <= max(2); ++i2) {
                    if (!op(FixedArray<TData, tndim>{ i0, i1, i2 })) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    template <class TResultData>
    AxisAlignedBoundingBox<TResultData, tndim> casted() const {
        return AxisAlignedBoundingBox<TResultData, tndim>(
            min.template casted<TResultData>(),
            max.template casted<TResultData>());
    }
    AxisAlignedBoundingBox operator + (const FixedArray<TData, tndim>& p) const {
        return { min + p, max + p };
    }
    AxisAlignedBoundingBox operator - (const FixedArray<TData, tndim>& p) const {
        return { min - p, max - p };
    }
    bool operator == (const AxisAlignedBoundingBox& other) const {
        return all(min == other.min) &&
               all(max == other.max);
    }
    Bound min;
    Bound max;
private:
    AxisAlignedBoundingBox(const FixedArray<TData, tndim>& min, const FixedArray<TData, tndim>& max)
        : min{ min }
        , max{ max }
    {}
    template <size_t tndim0, class TOperation>
    bool for_each_corner(
        const TOperation& op,
        FixedArray<TData, tndim>& corner) const
    {
        static_assert(tndim != 0);
        static_assert(tndim0 <= tndim);
        if constexpr (tndim0 == tndim) {
            return op(corner);
        } else {
            corner(tndim0) = min(tndim0);
            if (!for_each_corner<tndim0 + 1>(op, corner)) {
                return false;
            }
            corner(tndim0) = max(tndim0);
            if (!for_each_corner<tndim0 + 1>(op, corner)) {
                return false;
            }
            return true;
        }
    }
};

template <class TData, size_t tndim>
inline bool intersects(const AxisAlignedBoundingBox<TData, tndim>& a, const AxisAlignedBoundingBox<TData, tndim>& b) {
    return a.intersects(b);
}

template <class TData, size_t tndim>
inline bool intersects(const AxisAlignedBoundingBox<TData, tndim>& a, const FixedArray<TData, tndim>& b) {
    return a.contains(b);
}

template <class TData, size_t tndim>
inline auto aabb(const FixedArray<TData, tndim>& p)
{
    return AxisAlignedBoundingBox<TData, tndim>::from_point(p);
}

template <class TPosition, size_t tndim>
inline const auto& aabb(const AxisAlignedBoundingBox<TPosition, tndim>& a)
{
    return a;
}

template <class TData, size_t tndim>
inline auto diagonal_sizes(const FixedArray<TData, tndim>& p)
{
    return fixed_zeros<TData, tndim>();
}

template <class TPosition, size_t tndim>
inline auto diagonal_sizes(const AxisAlignedBoundingBox<TPosition, tndim>& a)
{
    return a.size();
}

template <class TData, size_t tndim>
FixedArray<TData, tndim> center(const FixedArray<TData, tndim>& p) {
    return p;
}

template <class TPosition, size_t tndim>
FixedArray<TPosition, tndim> center(const AxisAlignedBoundingBox<TPosition, tndim>& aabb) {
    return aabb.center();
}

template <class TData, size_t tndim>
void print(const FixedArray<TData, tndim>& p, std::ostream& ostr, size_t rec = 0) {
    std::string indent(rec, ' ');
    ostr << indent << p;
}

template <class TPosition, size_t tndim>
void print(const AxisAlignedBoundingBox<TPosition, tndim>& aabb, std::ostream& ostr, size_t rec = 0) {
    aabb.print(ostr, rec);
}

template <class TData, size_t tndim>
std::ostream& operator << (std::ostream& ostr, const AxisAlignedBoundingBox<TData, tndim>& aabb) {
    aabb.print(ostr);
    return ostr;
}

}

#ifdef __GNUC__
    #pragma GCC pop_options
#endif
