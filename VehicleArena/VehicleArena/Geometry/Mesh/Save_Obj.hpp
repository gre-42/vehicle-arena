// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Mesh/Indexed_Face_Set.hpp>
#include <VehicleArena/Geometry/Mesh/Obj_Material.hpp>
#include <VehicleArena/Geometry/Mesh/Save_Mtllib.hpp>
#include <VehicleArena/Math/Funpack.hpp>
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace VA {

template <class TPos>
class ColoredVertexArray;
template <class TPos>
class TriangleList;
struct Material;

template <class TDir, class TPos, class TIndex>
void save_obj(
    const std::string& filename,
    const IndexedFaceSet<TDir, TPos, TIndex>& data,
    const std::map<std::string, ObjMaterial>* materials)
{
    namespace fs = std::filesystem;
    static const DECLARE_REGEX(filename_re, "^(.*)\\.obj$");
    VA::re::cmatch match;
    if (!VA::re::regex_match(filename, match, filename_re)) {
        throw std::runtime_error("OBJ filename does not have \".obj\" extension: " + filename);
    }
    std::ofstream ostr{ filename };
    if constexpr (std::is_same_v<funpack_t<TPos>, double>) {
        ostr.precision(15);
    } else if constexpr (std::is_same_v<funpack_t<TPos>, float>) {
        ostr.precision(7);
    } else {
        throw std::runtime_error("Unsupported floating point type");
    }
    ostr << std::scientific;
    if (materials != nullptr) {
        std::string mtl_filename = match[1].str() + ".mtl";
        save_mtllib(mtl_filename, *materials);
        ostr << "mtllib " << fs::path(mtl_filename).filename().string()  << '\n';
    }
    for (const auto& v : data.vertices) {
        ostr << "v " << v << '\n';
    }
    for (const auto& uv : data.uvs) {
        ostr << "vt " << uv << '\n';
    }
    for (const auto& n : data.normals) {
        ostr << "vn " << n << '\n';
    }
    for (const NamedObjPolygons<TIndex>& named_polygons : data.named_obj_polygons) {
        if (!named_polygons.name.empty()) {
            ostr << "g " << named_polygons.name << '\n';
            ostr << "usemtl " << named_polygons.material_name << '\n';
        }
        auto add_polygons = [&ostr]<size_t tnvertices>(
            const UUVector<FixedArray<IndexVertex<TIndex>, tnvertices>>& polygons)
        {
            for (const auto& t : polygons) {
                ostr << "f ";
                size_t i = 0;
                for (const auto& v : t.flat_iterable()) {
                    if (i++ != 0) {
                        ostr << ' ';
                    }
                    ostr << (v.position + 1) << '/' << (v.uv + 1) << '/' << (v.normal + 1);
                }
                ostr << '\n';
            }
        };
        add_polygons(named_polygons.triangles);
        add_polygons(named_polygons.quads);
    }
    ostr.flush();
    if (ostr.fail()) {
        throw std::runtime_error("Could not write to file \"" + filename + '"');
    }
}

template <class TPos>
void save_obj(
    const std::string& filename,
    const std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas,
    const std::function<std::string(const ColoredVertexArray<TPos>&)>& material_name = {},
    const std::function<ObjMaterial(const Material&)>& convert_material = {});

template <class TPos>
void save_obj(
    const std::string& filename,
    const std::list<std::shared_ptr<TriangleList<TPos>>>& cvas,
    const std::function<std::string(const TriangleList<TPos>&)>& material_name = {},
    const std::function<ObjMaterial(const Material&)>& convert_material = {});

}
