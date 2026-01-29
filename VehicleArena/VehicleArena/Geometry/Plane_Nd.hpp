// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Line_Normal.hpp>
#include <VehicleArena/Geometry/Triangle_Normal.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Stats/Mean.hpp>
#include <ostream>

namespace VA {

template <class TDir, class TPos, size_t n>
class TransformationMatrix;

template <class TDir, class TPos, size_t tndim>
class PlaneNd {
public:
    PlaneNd(Uninitialized)
        : normal{ uninitialized }
    {}
    PlaneNd(const FixedArray<TDir, tndim>& normal, const TPos& intercept)
        : normal{ normal }
        , intercept{ intercept }
    {}
    PlaneNd(const FixedArray<TDir, tndim>& normal, const FixedArray<TPos, tndim>& point_on_plane)
        : PlaneNd{ normal, -(TPos)dot0d(normal.template casted<funpack_t<TPos>>(), funpack(point_on_plane)) }
    {}
    explicit PlaneNd(const FixedArray<TPos, 2, 2>& line, bool compute_center = false)
        : PlaneNd{ line_normal(funpack(line)).template casted<TDir>(), compute_center ? mean<0>(line) : line[0] }
    {}
    explicit PlaneNd(const FixedArray<TPos, 3, 3>& triangle, bool compute_center = false)
        : PlaneNd{ triangle_normal(funpack(triangle)).template casted<TDir>(), compute_center ? mean<0>(triangle) : triangle[0] }
    {}
    template <class TTPos>
    PlaneNd transformed(const TransformationMatrix<TDir, TTPos, tndim>& transformation_matrix) const {
        PlaneNd result = uninitialized;
        const auto& n0 = normal;
        const auto& i0 = intercept;
        auto& n1 = result.normal;
        auto& i1 = result.intercept;
        n1 = transformation_matrix.rotate(n0);
        i1 = i0 - (TPos)dot0d(n1.template casted<funpack_t<TTPos>>(), funpack(transformation_matrix.t));
        // i1 = -dot0d(n1, trafo(n0 * (-i0))) = -dot0d(n1, -i0 * n1 + t) = i0 - dot0d(n1, t)
        return result;
    }
    PlaneNd operator + (const FixedArray<TPos, tndim>& offset) const {
        using I = funpack_t<TPos>;
        auto n = normal.template casted<I>();
        auto o = offset.template casted<I>();
        return { normal, intercept - (TPos)dot0d(n, o) };
    }
    PlaneNd operator - (const FixedArray<TPos, tndim>& offset) const {
        return *this + (-offset);
    }
    PlaneNd operator - () const {
        return { -normal, -intercept };
    }
    template <class TDir2, class TPos2>
    PlaneNd<TDir2, TPos2, tndim> casted() const {
        return { normal.template casted<TDir2>(), TPos2(intercept) };
    }
    bool operator == (const PlaneNd& other) const {
        return all(normal == other.normal) &&
               (intercept == other.intercept);
    }
    FixedArray<TDir, tndim> normal;
    TPos intercept;
};

template <class TDir, class TPos, size_t tndim>
std::ostream& operator << (std::ostream& ostr, const PlaneNd<TDir, TPos, tndim>& plane) {
    ostr << "normal: " << plane.normal << " intercept: " << plane.intercept;
    return ostr;
}

}
