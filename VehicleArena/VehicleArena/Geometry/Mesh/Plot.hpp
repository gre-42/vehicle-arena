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
#include <string>
#include <vector>

namespace p2t {

struct Point;

}

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
template <class TPos>
struct ColoredVertex;
class PgmImage;
template <class TSize>
class Svg;
class PTri;
class StbImage3;

template <class TPos>
StbImage3 plot_mesh(
    const FixedArray<size_t, 2>& image_size,
    size_t line_thickness,
    size_t point_size,
    const std::list<FixedArray<TPos, 3, 2>>& triangles,
    const std::list<std::list<FixedArray<TPos, 2>>>& contours,
    const std::list<FixedArray<TPos, 2>>& highlighted_nodes,
    const std::list<FixedArray<TPos, 2>>& crossed_nodes);

template <class TPos>
StbImage3 plot_mesh(
    const FixedArray<size_t, 2>& image_size,
    size_t line_thickness,
    size_t point_size,
    const std::list<const FixedArray<ColoredVertex<TPos>, 3>*>& triangles,
    const std::list<std::list<FixedArray<TPos, 3>>>& contours,
    const std::list<FixedArray<TPos, 3>>& highlighted_nodes,
    const std::list<FixedArray<TPos, 3>>& crossed_nodes);

template <class TPos>
StbImage3 plot_mesh(
    const FixedArray<size_t, 2>& image_size,
    size_t line_thickness,
    size_t point_size,
    const std::list<FixedArray<TPos, 3, 2>>& triangles,
    const std::list<std::vector<FixedArray<TPos, 2>>>& contours,
    const std::list<FixedArray<TPos, 2>>& highlighted_nodes,
    const std::list<FixedArray<TPos, 2>>& crossed_nodes);

StbImage3 plot_mesh(
    const FixedArray<size_t, 2>& image_size,
    size_t line_thickness,
    size_t point_size,
    const std::list<PTri>& triangles,
    const std::list<std::vector<p2t::Point*>>& contours,
    const std::list<p2t::Point*>& highlighted_nodes,
    const std::list<p2t::Point*>& crossed_nodes);

template <class TPos>
void plot_mesh(
    Svg<double>& svg,
    const std::list<FixedArray<TPos, 3, 2>>& triangles,
    const std::list<FixedArray<TPos, 2, 2>>& edges,
    const std::list<std::list<FixedArray<TPos, 2>>>& contours,
    const std::list<FixedArray<TPos, 2>>& highlighted_nodes,
    TPos line_width = 0.05f);

template <class TPos>
void plot_mesh(
    Svg<double>& svg,
    const std::list<const FixedArray<ColoredVertex<TPos>, 3>*>& triangles,
    const std::list<std::list<FixedArray<TPos, 3>>>& contours,
    const std::list<FixedArray<TPos, 3>>& highlighted_nodes,
    TPos line_width = 0.05f);

void plot_mesh_svg(
    const std::string& filename,
    double width,
    double height,
    const std::list<const FixedArray<ColoredVertex<CompressedScenePos>, 3>*>& triangles,
    const std::list<std::list<FixedArray<CompressedScenePos, 3>>>& contours,
    const std::list<FixedArray<CompressedScenePos, 3>>& highlighted_nodes,
    CompressedScenePos line_width = (CompressedScenePos)0.5f);

void plot_mesh_svg(
    const std::string& filename,
    double width,
    double height,
    const std::list<FixedArray<CompressedScenePos, 3, 2>>& triangles,
    const std::list<FixedArray<CompressedScenePos, 2, 2>>& edges,
    const std::list<std::list<FixedArray<CompressedScenePos, 2>>>& contours,
    const std::list<FixedArray<CompressedScenePos, 2>>& highlighted_nodes,
    CompressedScenePos line_width = (CompressedScenePos)0.5f);

void plot_mesh_svg(
    const std::string& filename,
    double width,
    double height,
    const std::list<FixedArray<CompressedScenePos, 3, 2>>& triangles,
    const std::list<FixedArray<CompressedScenePos, 2, 2>>& edges,
    const std::list<std::vector<FixedArray<CompressedScenePos, 2>>>& contours,
    const std::list<FixedArray<CompressedScenePos, 2>>& highlighted_nodes,
    CompressedScenePos line_width = (CompressedScenePos)0.5f);

void plot_mesh_svg(
    const std::string& filename,
    double width,
    double height,
    const std::list<PTri>& triangles,
    const std::list<FixedArray<double, 2, 2>>& edges,
    const std::list<std::vector<p2t::Point*>>& contours,
    const std::list<p2t::Point*>& highlighted_nodes,
    double line_width = 0.05f);

}
