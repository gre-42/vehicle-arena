// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Smoothen_Edges.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Triangle_Normal.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <map>

using namespace VA;

template <class TPos>
void VA::smoothen_edges(
    std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas,
    const std::set<OrderableFixedArray<TPos, 3>>& excluded_vertices,
    float smoothness,
    size_t niterations,
    float decay)
{
    for (size_t i = 0; i < niterations; ++i) {
        typedef OrderableFixedArray<TPos, 3> Vertex3;
        typedef OrderableFixedArray<Vertex3, 2> Edge3;
        std::map<Edge3, Vertex3> edge_neighbors;
        std::map<Vertex3, FixedArray<float, 3>> vertex_movement;
        for (const auto& l : cvas) {
            for (const auto& t : l->triangles) {
                auto insert_edge = [&](size_t i, size_t j, size_t n){
                    Vertex3 ei{t(i).position};
                    Vertex3 ej{t(j).position};
                    Vertex3 nn{t(n).position};
                    auto it = edge_neighbors.find(Edge3{ej, ei});
                    if (it == edge_neighbors.end()) {
                        edge_neighbors.insert({Edge3{ei, ej}, nn});
                    } else {
                        using Triangle = FixedArray<TPos, 3, 3>;
                        FixedArray<float, 3> n0 = triangle_normal(funpack(Triangle{ej, ei, it->second})).template casted<float>();
                        FixedArray<float, 3> n1 = triangle_normal(funpack(Triangle{ei, ej, nn})).template casted<float>();
                        FixedArray<TPos, 3> cn = (it->second + nn) / operand<TPos, 2>;
                        FixedArray<TPos, 3> ce = (ei + ej) / operand<TPos, 2>;
                        FixedArray<float, 3> v = (cn - ce).template casted<float>();
                        FixedArray<float, 3> n01 = (n0 + n1) / 2.f;
                        n01 /= std::sqrt(sum(squared(n01)));
                        float n0n1 = dot0d(n0, n1);
                        if (n0n1 >= 0 && n0n1 < 1) {
                            float shift = std::sqrt(1 - squared(n0n1)) * sign(dot0d(v, n01));
                            if (auto e = ei; !excluded_vertices.contains(e)) {
                                auto it = vertex_movement.try_emplace(e, fixed_zeros<TPos, 3>()).first;
                                it->second += smoothness * n01 * shift;
                            }
                            if (auto e = ej; !excluded_vertices.contains(e)) {
                                auto it = vertex_movement.try_emplace(e, fixed_zeros<TPos, 3>()).first;
                                it->second += smoothness * n01 * shift;
                            }
                        }
                    }
                    };
                insert_edge(0, 1, 2);
                insert_edge(1, 2, 0);
                insert_edge(2, 0, 1);
            }
        }
        for (const auto& l : cvas) {
            for (auto& t : l->triangles) {
                for (auto& v : t.flat_iterable()) {
                    auto mit = vertex_movement.find(Vertex3(v.position));
                    if (mit != vertex_movement.end()) {
                        v.position += mit->second.template casted<TPos>();
                    }
                }
            }
        }
        smoothness *= decay;
    }
}

namespace VA {
    template void smoothen_edges<float>(
        std::list<std::shared_ptr<ColoredVertexArray<float>>>& cvas,
        const std::set<OrderableFixedArray<float, 3>>& excluded_vertices,
        float smoothness,
        size_t niterations,
        float decay);
    template void smoothen_edges<CompressedScenePos>(
        std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
        const std::set<OrderableFixedArray<CompressedScenePos, 3>>& excluded_vertices,
        float smoothness,
        size_t niterations,
        float decay);
}
