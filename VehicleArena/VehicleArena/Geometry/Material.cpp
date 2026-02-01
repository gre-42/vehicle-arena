// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Material.hpp"
#include <VehicleArena/Geometry/Morphology.hpp>
#include <stdexcept>

using namespace VA;

Material& Material::compute_color_mode() {
    for (auto& t : textures_color) {
        if (t.texture_descriptor.color.color_mode == ColorMode::UNDEFINED) {
            t.texture_descriptor.color.color_mode = (blend_mode == BlendMode::OFF)
                ? ColorMode::RGB
                : ColorMode::RGBA;
        }
    }
    return *this;
}

std::string Material::identifier() const {
    if (textures_color.size() > 0) {
        return "color: " + *textures_color.front().texture_descriptor.color.filename;
    } else {
        return "<no texture>";
    }
}

const BillboardAtlasInstance& Material::billboard_atlas_instance(
    BillboardId billboard_id,
    const std::string& name) const
{
    if (billboard_id >= billboard_atlas_instances.size()) {
        throw std::runtime_error(
            name + ": Billboard ID out of bounds in material \"" + identifier() + "\" (" +
            std::to_string(billboard_id) +
            " >= " +
            std::to_string(billboard_atlas_instances.size()) + ')');
    }
    return billboard_atlas_instances[billboard_id];
}

ScenePos Material::max_center_distance2(
    BillboardId billboard_id,
    const Morphology& morphology,
    const std::string& name) const
{
    return (billboard_id == BILLBOARD_ID_NONE)
        ? morphology.center_distances2(1)
        : billboard_atlas_instance(billboard_id, name).max_center_distance2;
}

ExternalRenderPassType Material::get_occluder_pass(
    BillboardId billboard_id,
    const std::string& name) const
{
    return (billboard_id == BILLBOARD_ID_NONE)
        ? occluder_pass
        : billboard_atlas_instance(billboard_id, name).occluder_pass;
}
