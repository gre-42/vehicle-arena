// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Save_Obj.hpp"
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Obj_Material.hpp>
#include <VehicleArena/Geometry/Mesh/Triangle_List.hpp>
#include <set>

using namespace VA;

template <class TPos, class TContainer>
void save_obj_with_materials(
    const std::string& filename,
    const std::list<std::shared_ptr<TContainer>>& cvas,
    const std::function<std::string(const TContainer&)>& material_name,
    const std::function<ObjMaterial(const Material&)>& convert_material)
{
    std::set<std::string> names;
    std::map<Material, size_t> material_indices;
    std::map<std::string, ObjMaterial> obj_materials;
    std::vector<NamedInputPolygons<
        decltype(cvas.front()->triangles),
        decltype(cvas.front()->quads)>> ipolys;
    ipolys.reserve(cvas.size());
    for (const std::shared_ptr<TContainer>& cva : cvas) {
        if (cva->name.empty()) {
            if (!cva->material.textures_color.empty()) {
                throw std::runtime_error("Empty name, material: \"" + *cva->material.textures_color.front().texture_descriptor.color.filename);
            } else {
                throw std::runtime_error("Empty name, no material color texture");
            }
        }
        if (!names.insert(cva->name.full_name()).second) {
            throw std::runtime_error("Duplicate name: \"" + cva->name.full_name() + '"');
        }
        std::string mname;
        if (material_name) {
            mname = material_name(*cva);
        } else {
            material_indices.insert({ cva->material, material_indices.size() });
            mname = cva->name.full_name() + "_material_" + std::to_string(material_indices.at(cva->material));
        }
        ObjMaterial obj_material;
        if (convert_material) {
            obj_material = convert_material(cva->material);
        } else {
            obj_material = ObjMaterial{
                .ambient = cva->material.shading.ambient,
                .diffuse = cva->material.shading.diffuse,
                .specular = cva->material.shading.specular
            };
        }
        ipolys.push_back({ cva->name.full_name(), mname, cva->triangles, cva->quads });
        obj_materials.insert({ mname, obj_material });
    }
    save_obj(filename, IndexedFaceSet<float, TPos, size_t>{ ipolys }, &obj_materials);
}

template <class TPos>
void VA::save_obj(
    const std::string& filename,
    const std::list<std::shared_ptr<ColoredVertexArray<TPos>>>& cvas,
    const std::function<std::string(const ColoredVertexArray<TPos>&)>& material_name,
    const std::function<ObjMaterial(const Material&)>& convert_material)
{
    save_obj_with_materials<TPos>(filename, cvas, material_name, convert_material);
}

template <class TPos>
void VA::save_obj(
    const std::string& filename,
    const std::list<std::shared_ptr<TriangleList<TPos>>>& cvas,
    const std::function<std::string(const TriangleList<TPos>&)>& material_name,
    const std::function<ObjMaterial(const Material&)>& convert_material)
{
    save_obj_with_materials<TPos>(filename, cvas, material_name, convert_material);
}

template void VA::save_obj<float>(
    const std::string& filename,
    const std::list<std::shared_ptr<ColoredVertexArray<float>>>& cvas,
    const std::function<std::string(const ColoredVertexArray<float>&)>& material_name,
    const std::function<ObjMaterial(const Material&)>& convert_material);

template void VA::save_obj<CompressedScenePos>(
    const std::string& filename,
    const std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
    const std::function<std::string(const ColoredVertexArray<CompressedScenePos>&)>& material_name,
    const std::function<ObjMaterial(const Material&)>& convert_material);

template void VA::save_obj<float>(
    const std::string& filename,
    const std::list<std::shared_ptr<TriangleList<float>>>& cvas,
    const std::function<std::string(const TriangleList<float>&)>& material_name,
    const std::function<ObjMaterial(const Material&)>& convert_material);

template void VA::save_obj<CompressedScenePos>(
    const std::string& filename,
    const std::list<std::shared_ptr<TriangleList<CompressedScenePos>>>& cvas,
    const std::function<std::string(const TriangleList<CompressedScenePos>&)>& material_name,
    const std::function<ObjMaterial(const Material&)>& convert_material);
