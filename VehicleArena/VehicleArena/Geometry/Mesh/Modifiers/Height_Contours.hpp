// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <list>
#include <map>
#include <memory>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
template <typename TData, size_t... tshape>
class OrderableFixedArray;
template <class TPos>
class ColoredVertexArray;
template <class TPos>
class TriangleList;

std::list<std::list<FixedArray<CompressedScenePos, 2>>> height_contours_by_vertex(
    const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
    CompressedScenePos height);

std::list<std::list<FixedArray<CompressedScenePos, 2>>> height_contours_by_vertex(
    const std::list<std::shared_ptr<TriangleList<CompressedScenePos>>>& cvas,
    CompressedScenePos height);

std::list<std::list<FixedArray<CompressedScenePos, 2>>> height_contours_by_edge(
    const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
    CompressedScenePos height);


std::list<std::list<FixedArray<CompressedScenePos, 2>>> height_contours_by_edge(
    const std::list<std::shared_ptr<TriangleList<CompressedScenePos>>>& cvas,
    CompressedScenePos height);

class HeightContoursByVertex {
public:
    explicit HeightContoursByVertex(CompressedScenePos height);
    ~HeightContoursByVertex();
    void add_triangle(const FixedArray<CompressedScenePos, 3, 3>& triangle);
    std::list<std::list<FixedArray<CompressedScenePos, 2>>> get_contours_and_clear();
private:
    CompressedScenePos height_;
    std::map<OrderableFixedArray<CompressedScenePos, 2>, FixedArray<CompressedScenePos, 2>> neighbors_;
};

class HeightContoursByEdge {
    using Edge = std::pair<OrderableFixedArray<CompressedScenePos, 3>, OrderableFixedArray<CompressedScenePos, 3>>;
public:
    explicit HeightContoursByEdge(CompressedScenePos height);
    ~HeightContoursByEdge();
    void add_triangle(const FixedArray<CompressedScenePos, 3, 3>& triangle);
    std::list<std::list<FixedArray<CompressedScenePos, 2>>> get_contours_and_clear();
private:
    static Edge swap(const Edge& e);
    CompressedScenePos height_;
    std::map<Edge, Edge> edge_neighbors_;
};

}
