// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Save_Mtllib.hpp"
#include <VehicleArena/Geometry/Mesh/Obj_Material.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <fstream>

using namespace VA;

void VA::save_mtllib(
    const std::string& filename,
    const std::map<std::string, ObjMaterial>& materials)
{
    std::ofstream ostr(filename);
    ostr.precision(15);
    ostr << std::scientific;
    for (const auto& [name, material] : materials) {
        ostr << "newmtl " << name << '\n';
        if (all(material.ambient != -1.f)) {
            ostr << "Ka " << material.ambient << '\n';
        }
        if (all(material.diffuse != -1.f)) {
            ostr << "Kd " << material.diffuse << '\n';
        }
        if (all(material.specular != -1.f)) {
            ostr << "Ks " << material.specular << '\n';
        }
        if (!material.color_texture.empty()) {
            ostr << "map_Kd " << material.color_texture << '\n';
        }
        if (!material.bump_texture.empty()) {
            ostr << "map_Bump " << material.bump_texture << '\n';
        }
        if (material.has_alpha_texture) {
            ostr << "map_d " << material.color_texture << '\n';
        }
    }
    ostr.flush();
    if (ostr.fail()) {
        throw std::runtime_error("Could not write to file \"" + filename + '"');
    }
}
