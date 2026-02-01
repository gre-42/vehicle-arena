// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Normal_Vector_Error_Behavior.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <map>

namespace VA {

enum class NormalVectorErrorBehavior;

template <class TPos, class TNormal>
class VertexNormals {
public:
    template <class TIterator>
    void add_triangles(const TIterator& begin, const TIterator& end) {
        for (auto it = begin; it != end; ++it) {
            add_triangle(*it);
        }
    }
    void add_triangle(const FixedArray<ColoredVertex<TPos>, 3>& triangle) {
        for (auto& v : triangle.flat_iterable()) {
            add_vertex_face_normal(v.position, v.normal);
        }
    }
    void add_vertex_face_normal(
        const FixedArray<TPos, 3>& position,
        const FixedArray<TNormal, 3>& normal)
    {
        auto it = vertices_.try_emplace(make_orderable(position), TNormal(0)).first;
        it->second += normal;
    }
    void compute_vertex_normals(NormalVectorErrorBehavior zero_behavior) {
        auto zb = zero_behavior;
        for (auto& [_, n] : vertices_) {
            auto len = std::sqrt(sum(squared(n)));
            if (len < 1e-12) {
                if (any(zb & NormalVectorErrorBehavior::WARN)) {
                    lwarn() << "Normal is zero";
                    zb = NormalVectorErrorBehavior::ZERO;
                }
                if (any(zb & NormalVectorErrorBehavior::THROW)) {
                    throw std::runtime_error("Normal is zero");
                }
                n = 0.f;
            } else {
                n /= len;
            }
        }
    }
    inline const FixedArray<TNormal, 3>& get_normal(const FixedArray<TPos, 3>& position) {
        return vertices_.at(make_orderable(position));
    }
    inline const std::map<OrderableFixedArray<TPos, 3>, FixedArray<TNormal, 3>>& vertices() {
        return vertices_;
    }
    template <size_t tnvertices>
    FixedArray<TNormal, tnvertices, 3> get_normals(
        const FixedArray<TPos, tnvertices, 3>& position)
    {
        FixedArray<TNormal, tnvertices, 3> result = uninitialized;
        for (size_t r = 0; r < tnvertices; ++r) {
            result[r] = get_normal(position[r]);
        }
        return result;
    }
private:
    std::map<OrderableFixedArray<TPos, 3>, FixedArray<TNormal, 3>> vertices_;
};

}
