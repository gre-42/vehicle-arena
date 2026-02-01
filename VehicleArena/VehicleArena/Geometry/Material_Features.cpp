// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Material_Features.hpp"
#include <VehicleArena/Geometry/Material/Blend_Distances.hpp>
#include <VehicleArena/Geometry/Material/Blend_Map_Texture.hpp>
#include <VehicleArena/Geometry/Material/Colormap_With_Modifiers_Hash.hpp>

using namespace VA;


bool VA::has_normalmap(const std::vector<BlendMapTexture>& textures_color) {
    for (const auto& t : textures_color) {
        if (!t.texture_descriptor.normal.filename->empty()) {
            return true;
        }
    }
    return false;
}

bool VA::fragments_depend_on_distance(
    const FixedArray<float, 2>& fog_distances,
    const FixedArray<float, 4>& alpha_distances,
    const std::vector<BlendMapTextureAndId>& textures_color)
{
    if (any(fog_distances != default_step_distances)) {
        return true;
    }
    if (any(alpha_distances != default_linear_distances)) {
        return true;
    }
    for (const auto& t : textures_color) {
        if (t.ops->distances != default_linear_distances) {
            return true;
        }
    }
    return false;
}

bool VA::fragments_depend_on_normal(const std::vector<BlendMapTexture>& textures_color)
{
    for (const auto& t : textures_color) {
        if (t.cosines != default_linear_cosines) {
            return true;
        }
    }
    return false;
}

bool VA::fragments_depend_on_normal(const std::vector<BlendMapTextureAndId>& textures_color)
{
    for (const auto& t : textures_color) {
        if (t.ops->cosines != default_linear_cosines) {
            return true;
        }
    }
    return false;
}

bool VA::has_horizontal_detailmap(const std::vector<BlendMapTextureAndId>& textures)
{
    for (const auto& t : textures) {
        if (any(t.ops->uv_source & BlendMapUvSource::ANY_HORIZONTAL)) {
            return true;
        }
    }
    return false;
}

bool ColormapPtr::operator == (const ColormapPtr& other) const {
    return *cm_ == *other.cm_;
}

std::size_t std::hash<VA::ColormapPtr>::operator() (const VA::ColormapPtr& k) const {
    auto hasher = std::hash<VA::ColormapWithModifiers>();
    return hasher(*k);
}
