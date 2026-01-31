// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Math/Sqrt.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <stdexcept>

namespace VA {

template <class TDir, class TPos, size_t tndim>
class PlaneNd;
template <class TPos, size_t tndim>
class BoundingSphere;
template <class TData, size_t tndim>
class AxisAlignedBoundingBox;
template <class TDir, class TPos, size_t tnvertices>
class ConvexPolygon3D;

template <class TDir, class TPos>
class RaySegment3D {
public:
    RaySegment3D(
        const FixedArray<TPos, 3>& start,
        const FixedArray<TDir, 3>& direction,
        const TDir& length)
        : start{ start }
        , direction{ direction }
        , length{ length }
    {}
    RaySegment3D(
        const FixedArray<TPos, 3>& start,
        const FixedArray<TPos, 3>& end)
        : start{ start }
        , direction{ uninitialized }
    {
        auto dir = funpack(end - start);
        auto l2 = sum(squared(dir));
        if (l2 < 0x1p-40) {
            throw std::runtime_error("Could not calculate ray direction");
        }
        length = (TDir)std::sqrt(l2);
        direction = dir.template casted<TDir>() / length;
    }
    explicit RaySegment3D(const FixedArray<TPos, 2, 3>& vertices)
        : RaySegment3D{ vertices[0], vertices[1] }
    {}
    bool intersects(const PlaneNd<TDir, TPos, 3>& plane, TPos* t, FixedArray<TPos, 3>* intersection_point) const {
        auto c = dot0d(plane.normal, direction);
        if (std::abs(c) < 1e-12) {
            return false;
        }
        *t = -(dot0d(plane.normal, start) + plane.intercept) / c;
        if (((*t) >= 0) && ((*t) <= length)) {
            *intersection_point = start + direction * (*t);
            return true;
        }
        return false;
    }
    template <size_t tnvertices>
    bool intersects(
        const ConvexPolygon3D<TDir, TPos, tnvertices>& polygon,
        TPos* t,
        FixedArray<TPos, 3>* intersection_point) const
    {
        if (!intersects(polygon.plane, t, intersection_point)) {
            return false;
        }
        return polygon.contains(*intersection_point);
    }
    bool intersects_slow(const AxisAlignedBoundingBox<TPos, 3>& aabb) const {
        if (aabb.contains(start) || aabb.contains(stop())) {
            return true;
        }
        auto interscts = [&](size_t axis0, size_t axis1, size_t axis2, bool mm) {
            TPos t;
            FixedArray<TPos, 3> intersection_point;
            PlaneNd<TDir, TPos, 3> plane;
            plane.normal(axis0) = 1;
            plane.normal(axis1) = 0;
            plane.normal(axis2) = 0;
            if (mm) {
                plane.intercept = -aabb.min(axis0);
            } else {
                plane.intercept = -aabb.max(axis0);
            }
            if (intersects(plane, &t, &intersection_point)) {
                if ((intersection_point(axis1) >= aabb.min(axis1)) && (intersection_point(axis1) <= aabb.max(axis1)) &&
                    (intersection_point(axis2) >= aabb.min(axis2)) && (intersection_point(axis2) <= aabb.max(axis2)))
                {
                    return true;
                }
            }
            return false;
            };
        return
            interscts(0, 1, 2, false) || interscts(1, 2, 0, false) || interscts(2, 0, 1, false) ||
            interscts(0, 1, 2, true) || interscts(1, 2, 0, true) || interscts(2, 0, 1, true);
    }
    FixedArray<TPos, 3> stop() const {
        return start + (direction * length).template casted<TPos>();
    }
    FixedArray<TPos, 3> center() const {
        return start + (direction * (length / TDir(2))).template casted<TPos>();
    }
    RaySegment3D<TDir, TPos> transformed(
        const TransformationMatrix<SceneDir, ScenePos, 3>& transformation_matrix) const
    {
        return {
            transformation_matrix.transform(start.template casted<ScenePos>()).template casted<TPos>(),
            transformation_matrix.casted<TDir, TDir>().rotate(direction),
            length
        };
    }
    template <class TResultDir, class TResultPos>
    RaySegment3D<TResultDir, TResultPos> casted() const {
        return {
            start.template casted<TResultPos>(),
            direction.template casted<TResultDir>(),
            (TResultDir)length
        };
    }
    bool operator == (const RaySegment3D& other) const {
        return all(start == other.start) &&
               all(direction == other.direction) &&
               (length == other.length);
    }
    FixedArray<TPos, 3> start;
    FixedArray<TDir, 3> direction;
    TDir length;
};

template <class TDir, class TPos>
RaySegment3D<TDir, TPos> operator + (
    const RaySegment3D<TDir, TPos>& ray,
    const FixedArray<TPos, 3>& p)
{
    return { ray.start + p, ray.direction, ray.length };
}

template <class TDir, class TPos>
RaySegment3D<TDir, TPos> operator - (
    const RaySegment3D<TDir, TPos>& ray,
    const FixedArray<TPos, 3>& p)
{
    return ray + (-p);
}

}
