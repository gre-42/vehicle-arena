// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Height_Contours.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Exceptions/Edge_Exception.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Contour.hpp>
#include <VehicleArena/Geometry/Mesh/Triangle_List.hpp>
#include <VehicleArena/Math/Lerp.hpp>
#include <map>

using namespace VA;

HeightContoursByEdge::Edge HeightContoursByEdge::swap(const Edge& e) {
    return Edge{e.second, e.first};
}

FixedArray<CompressedScenePos, 2> intersection_point(
    const FixedArray<CompressedScenePos, 3>& a,
    const FixedArray<CompressedScenePos, 3>& b,
    CompressedScenePos height)
{
    auto a2 = FixedArray<CompressedScenePos, 2>{a(0), a(1)};
    auto b2 = FixedArray<CompressedScenePos, 2>{b(0), b(1)};
    auto dh = funpack(b(2) - a(2));
    if (std::abs(dh) < 1e-12) {
        return { a(0), a(1) };
    } else if (dh > 0) {
        return lerp(a2, b2, funpack(height - a(2)) / dh);
    } else {
        return lerp(b2, a2, funpack(height - b(2)) / (-dh));
    }
}

std::list<std::list<FixedArray<CompressedScenePos, 2>>> VA::height_contours_by_edge(
    const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
    CompressedScenePos height)
{
    HeightContoursByEdge hc{ height };
    for (const auto& cva : cvas) {
        for (const auto& t : cva->triangles) {
            hc.add_triangle(FixedArray<CompressedScenePos, 3, 3>{
                t(0).position,
                t(1).position,
                t(2).position});
        }
    }
    return hc.get_contours_and_clear();
}

std::list<std::list<FixedArray<CompressedScenePos, 2>>> VA::height_contours_by_edge(
    const std::list<std::shared_ptr<TriangleList<CompressedScenePos>>>& cvas,
    CompressedScenePos height)
{
    HeightContoursByEdge hc{ height };
    for (const auto& cva : cvas) {
        for (const auto& t : cva->triangles) {
            hc.add_triangle(FixedArray<CompressedScenePos, 3, 3>{
                t(0).position,
                t(1).position,
                t(2).position});
        }
    }
    return hc.get_contours_and_clear();
}

HeightContoursByEdge::HeightContoursByEdge(CompressedScenePos height)
    : height_{ height }
{}

HeightContoursByEdge::~HeightContoursByEdge() = default;

void HeightContoursByEdge::add_triangle(const FixedArray<CompressedScenePos, 3, 3>& triangle)
{
    // linfo() << "triangle";
    for (size_t i = 0; i < 3; ++i) {
        Edge es[] = {
            Edge{
                triangle[i],
                triangle[(i + 1) % 3]
            },
            Edge{
                triangle[(i + 1) % 3],
                triangle[(i + 2) % 3]
            }
        };
        const auto& center = triangle[(i + 1) % 3];
        if (![&](){
            for (auto& e : es) {
                {
                    auto mi = std::min(e.first(2), e.second(2));
                    auto ma = std::max(e.first(2), e.second(2));
                    if ((mi > height_) || (ma < height_)) {
                        return false;
                    }
                }
                if ((e.first(2) == height_) && (e.second(2) == height_)) {
                    // Do nothing
                } else if (e.first(2) == height_) {
                    e.second = e.first;
                } else if (e.second(2) == height_) {
                    e.first = e.second;
                }
            }
            return true;
        }()){
            continue;
        }
        // linfo() << h << " - " << a << " - " << b;
        auto add_neighbor = [this](
            const Edge& ne0,
            const Edge& ne1)
        {
            // linfo() << "Add neighbor " << v0 << " -> " << v1;
            auto res = edge_neighbors_.try_emplace(ne0, ne1);
            if (!res.second && (res.first->second != ne1)) {
                // throw EdgeException{v0, v1, "Vertex has multiple neighbors"};
                throw EdgeException{ne0.first, ne0.second, "Edge has multiple neighbors"};
            }
        };
        if (es[0] != es[1]) {
            if (center(2) < height_) {
                add_neighbor(es[1], swap(es[0]));
            } else {
                add_neighbor(es[0], swap(es[1]));
            }
        }
    }
}

std::list<std::list<FixedArray<CompressedScenePos, 2>>> HeightContoursByEdge::get_contours_and_clear()
{
    // while (std::erase_if(edge_neighbors, [&](const auto& item){
    //         const auto& [key, value] = item;
    //         return !edge_neighbors.contains(value);
    //     }) != 0);
    auto ecs = find_neighbor_contours<Edge>(
        edge_neighbors_,
        [&](const Edge& e){ return intersection_point(e.first, e.second, height_); });
    std::list<std::list<FixedArray<CompressedScenePos, 2>>> pcs;
    for (const auto& ec : ecs) {
        auto& c = pcs.emplace_back();
        std::optional<FixedArray<CompressedScenePos, 2>> p0;
        for (const auto& e : ec) {
            auto p1 = intersection_point(e.first, e.second, height_);
            if (p0.has_value()) {
                if (sum(squared(p1 - *p0)) > 1e-12) {
                    c.push_back(p1);
                }
            } else {
                c.push_back(p1);
            }
            p0 = p1;
        }
    }
    return pcs;
}

std::list<std::list<FixedArray<CompressedScenePos, 2>>> VA::height_contours_by_vertex(
    const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
    CompressedScenePos height)
{
    HeightContoursByVertex hc{ height };
    for (const auto& cva : cvas) {
        for (const auto& t : cva->triangles) {
            hc.add_triangle(FixedArray<CompressedScenePos, 3, 3>{
                t(0).position,
                t(1).position,
                t(2).position});
        }
    }
    return hc.get_contours_and_clear();
}

std::list<std::list<FixedArray<CompressedScenePos, 2>>> VA::height_contours_by_vertex(
    const std::list<std::shared_ptr<TriangleList<CompressedScenePos>>>& cvas,
    CompressedScenePos height)
{
    HeightContoursByVertex hc{ height };
    for (const auto& cva : cvas) {
        for (const auto& t : cva->triangles) {
            hc.add_triangle(FixedArray<CompressedScenePos, 3, 3>{
                t(0).position,
                t(1).position,
                t(2).position});
        }
    }
    return hc.get_contours_and_clear();
}

HeightContoursByVertex::HeightContoursByVertex(CompressedScenePos height)
    : height_{ height }
{}

HeightContoursByVertex::~HeightContoursByVertex() = default;

void HeightContoursByVertex::add_triangle(const FixedArray<CompressedScenePos, 3, 3>& triangle) {
    // linfo() << "triangle";
    auto n = triangle_normal(funpack(triangle));
    auto ln = FixedArray<ScenePos, 2>{n(1), -n(0)};
    // linfo() << "n " << n;
    std::optional<FixedArray<CompressedScenePos, 2>> p0;
    for (size_t i = 0; i < 3; ++i) {
        const auto& a = triangle[i];
        const auto& b = triangle[(i + 1) % 3];
        {
            auto mi = std::min(a(2), b(2));
            auto ma = std::max(a(2), b(2));
            if ((mi > height_) || (ma < height_)) {
                continue;
            }
        }
        // linfo() << h << " - " << a << " - " << b;
        auto p1 = intersection_point(a, b, height_);
        auto add_neighbor = [&](
            const FixedArray<CompressedScenePos, 2>& v0,
            const FixedArray<CompressedScenePos, 2>& v1)
        {
            // linfo() << "Add neighbor " << v0 << " -> " << v1;
            auto res = neighbors_.try_emplace(OrderableFixedArray<CompressedScenePos, 2>{v0}, v1);
            if (!res.second && any(res.first->second != v1)) {
                // throw EdgeException{v0, v1, "Vertex has multiple neighbors"};
                throw EdgeException{a, b, "Vertex has multiple neighbors"};
            }
        };
        auto add_edge = [&](
            const FixedArray<CompressedScenePos, 2>& v0,
            const FixedArray<CompressedScenePos, 2>& v1)
        {
            auto d = v1 - v0;
            auto dir = dot0d(ln, funpack(d));
            // linfo() << "dir " << dir << " - " << ln << " - " << d << " - " << a << " - " << b;
            if ((std::abs(dir) < 1e-12) || (dir < 0)) {
                add_neighbor(v0, v1);
            } else {
                add_neighbor(v1, v0);
            }
        };
        if (p0.has_value() && any(*p0 != p1)) {
            add_edge(*p0, p1);
        }
        p0 = p1;
    }
}

std::list<std::list<FixedArray<CompressedScenePos, 2>>> HeightContoursByVertex::get_contours_and_clear() {
    return find_neighbor_contours<FixedArray<CompressedScenePos, 2>>(neighbors_);
}
