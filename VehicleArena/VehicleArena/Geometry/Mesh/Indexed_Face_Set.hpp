// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Mesh/Vertex_Normals.hpp>
#include <VehicleArena/Initialization/Default_Uninitialized_Vector.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <list>
#include <string>

namespace VA {

template <class TIndex>
struct IndexVertex {
    TIndex position;
    TIndex uv;
    TIndex normal;
};

template <class TInputTriangles, class TInputQuads>
struct NamedInputPolygons {
    std::string name;
    std::string material_name;
    const TInputTriangles& triangles;
    const TInputQuads& quads;
};

template <class TIndex>
struct NamedObjPolygons {
    std::string name;
    std::string material_name;
    UUVector<FixedArray<IndexVertex<TIndex>, 3>> triangles;
    UUVector<FixedArray<IndexVertex<TIndex>, 4>> quads;
};

template <class TDir, class TPos, class TIndex>
class IndexedFaceSet {
    using NamedListInputPolygons = NamedInputPolygons<
        std::list<FixedArray<ColoredVertex<TPos>, 3>>,
        std::list<FixedArray<ColoredVertex<TPos>, 4>>>;
    using NamedVectorInputPolygons = NamedInputPolygons<
        UUVector<FixedArray<ColoredVertex<TPos>, 3>>,
        UUVector<FixedArray<ColoredVertex<TPos>, 4>>>;
public:
    IndexedFaceSet(
        const std::list<FixedArray<ColoredVertex<TPos>, 3>>& triangles,
        const std::list<FixedArray<ColoredVertex<TPos>, 4>>& quads = {})
    : IndexedFaceSet{std::vector<NamedListInputPolygons>{NamedListInputPolygons{"", "", triangles, quads}}}
    {}
    IndexedFaceSet(
        const UUVector<FixedArray<ColoredVertex<TPos>, 3>>& triangles,
        const UUVector<FixedArray<ColoredVertex<TPos>, 4>>& quads = {})
    : IndexedFaceSet{std::vector<NamedVectorInputPolygons>{NamedVectorInputPolygons{"", "", triangles, quads}}}
    {}
    template <class TInputTriangles, class TInputQuads>
    IndexedFaceSet(const std::vector<NamedInputPolygons<TInputTriangles, TInputQuads>>& named_input_polygons)
    {
        std::map<OrderableFixedArray<TPos, 3>, size_t> vertex_indices;
        std::map<OrderableFixedArray<TDir, 3>, size_t> normal_indices;
        std::map<OrderableFixedArray<TDir, 2>, size_t> uv_indices;
        named_obj_polygons.reserve(named_input_polygons.size());
        for (const NamedInputPolygons<TInputTriangles, TInputQuads>& named_ipolys : named_input_polygons) {
            auto& named_opolys = named_obj_polygons.emplace_back(NamedObjPolygons<TIndex>{named_ipolys.name, named_ipolys.material_name, {}, {}});
            named_opolys.triangles.reserve(named_ipolys.triangles.size());
            named_opolys.quads.reserve(named_ipolys.quads.size());
            auto add_polygons = [&](const auto& ipolys, auto& opolys)
            {
                for (const auto& ipoly : ipolys) {
                    for (const auto& v : ipoly.flat_iterable()) {
                        vertex_indices.insert({ make_orderable(v.position), vertex_indices.size() });
                        uv_indices.insert({ make_orderable(v.uv), uv_indices.size() });
                        normal_indices.insert({ make_orderable(v.normal), normal_indices.size() });
                    }
                    FixedArray<IndexVertex<TIndex>, std::remove_reference_t<decltype(ipoly)>::length()> opolygon{ uninitialized };
                    for (size_t i = 0; i < CW::length(ipoly); ++i) {
                        opolygon(i).position = vertex_indices.at(make_orderable(ipoly(i).position));
                        opolygon(i).uv = uv_indices.at(make_orderable(ipoly(i).uv));
                        opolygon(i).normal = normal_indices.at(make_orderable(ipoly(i).normal));
                    }
                    opolys.emplace_back(opolygon);
                }
            };
            add_polygons(named_ipolys.triangles, named_opolys.triangles);
            add_polygons(named_ipolys.quads, named_opolys.quads);
        }
        vertices.resize(vertex_indices.size());
        for (const auto& v : vertex_indices) {
            vertices[v.second] = v.first;
        }
        uvs.resize(uv_indices.size());
        for (const auto& v : uv_indices) {
            uvs[v.second] = v.first;
        }
        normals.resize(normal_indices.size());
        for (const auto& v : normal_indices) {
            normals[v.second] = v.first;
        }
    }
    UUVector<FixedArray<TPos, 3>> vertices;
    UUVector<FixedArray<TDir, 2>> uvs;
    UUVector<FixedArray<TDir, 3>> normals;
    UVector<NamedObjPolygons<TIndex>> named_obj_polygons;
};

}
