// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Triangle_Normal.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <list>
#include <map>

namespace VA {

template <class TPos, class TNormal>
class VertexNormalsWithSeams {
public:
    template <class TIterator>
    void add_triangles(const TIterator& begin, const TIterator& end) {
        for (auto it = begin; it != end; ++it) {
            add_triangle(*it);
        }
    }
    void add_triangle(const FixedArray<ColoredVertex<TPos>, 3>& triangle) {
        using Triangle = FixedArray<TPos, 3, 3>;
        auto normal = triangle_normal(funpack(Triangle{
            triangle(0).position,
            triangle(1).position,
            triangle(2).position })).template casted<TNormal>();
        for (auto& v : triangle.flat_iterable()) {
            add_vertex_face_normal(v.position, normal);
        }
    }
    void add_vertex_face_normal(
        const FixedArray<TPos, 3>& position,
        const FixedArray<TNormal, 3>& normal)
    {
        normals_[make_orderable(position)].push_back(normal);
    }
    FixedArray<TNormal, 3> get_normal(
        const FixedArray<TPos, 3>& position,
        const FixedArray<TNormal, 3>& reference_normal,
        const TNormal& seam_threshold)
    {
        FixedArray<TNormal, 3> result((TNormal)0);
        for (const auto& n : normals_.at(make_orderable(position))) {
            if (dot0d(n, reference_normal) > seam_threshold) {
                result += n;
            }
        }
        auto l2 = sum(squared(result));
        if (l2 < 1e-12) {
            throw std::runtime_error("VertexNormalsWithSeams: Could not find a single normal, or normals are opposing");
        }
        return result / std::sqrt(l2);
    }
    FixedArray<ColoredVertex<TPos>, 3> triangle(
        const FixedArray<ColoredVertex<TPos>, 3>& t,
        const TNormal& seam_threshold)
    {
        using Triangle = FixedArray<TPos, 3, 3>;
        auto normal = triangle_normal(funpack(Triangle{
            t(0).position,
            t(1).position,
            t(2).position })).template casted<TNormal>();
        auto tri = t;
        for (size_t i = 0; i < 3; ++i) {
            tri(i).normal = get_normal(tri(i).position, normal, seam_threshold);
        }
        return tri;
    }
private:
    std::list<FixedArray<ColoredVertex<TPos>, 3>> triangles_;
    std::map<OrderableFixedArray<TPos, 3>, std::list<FixedArray<TNormal, 3>>> normals_;
};

}
