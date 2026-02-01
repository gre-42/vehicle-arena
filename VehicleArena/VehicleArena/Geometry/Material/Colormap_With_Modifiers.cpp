// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Colormap_With_Modifiers.hpp"
#include <VehicleArena/Geometry/Material/Colormap_With_Modifiers_Hash.hpp>
#include <VehicleArena/Hashing/Hash.hpp>

using namespace VA;

std::partial_ordering ColormapWithModifiers::operator <=> (const ColormapWithModifiers& other) const {
    if (!hash.has_value()) {
        throw std::runtime_error("Hash not computed for colormap \"" + *filename + '"');
    }
    if (!other.hash.has_value()) {
        throw std::runtime_error("Hash not computed for colormap \"" + *other.filename + '"');
    }
    return *hash <=> *other.hash;
}

bool ColormapWithModifiers::operator == (const ColormapWithModifiers& other) const {
    if (!hash.has_value()) {
        throw std::runtime_error("Hash not computed for colormap \"" + *filename + '"');
    }
    if (!other.hash.has_value()) {
        throw std::runtime_error("Hash not computed for colormap \"" + *other.filename + '"');
    }
    return *hash == *other.hash;
}

ColormapWithModifiers& ColormapWithModifiers::compute_hash() {
    hash = VA::hash_combine(
        filename,
        desaturate,
        desaturation_exponent,
        alpha,
        histogram,
        average,
        multiply,
        alpha_blend,
        mean_color,
        lighten,
        lighten_left,
        lighten_right,
        lighten_top,
        lighten_bottom,
        selected_color,
        selected_color_near,
        selected_color_far,
        color_to_replace,
        replacement_color,
        replacement_tolerance,
        edge_sigma,
        times,
        plus,
        abs,
        invert,
        height_to_normals,
        saturate,
        multiply_with_alpha,
        color_mode,
        alpha_fac,
        alpha_exponent,
        mipmap_mode,
        magnifying_interpolation_mode,
        depth_interpolation,
        anisotropic_filtering_level,
        wrap_modes,
        border_color,
        rotate);
    return *this;
}

std::ostream& VA::operator << (std::ostream& ostr, const ColormapWithModifiers& t) {
    ostr <<
        "filename: " << *t.filename << '\n' <<
        "desaturate: " << (int)t.desaturate << '\n' <<
        "desaturation_exponent: " << (int)t.desaturation_exponent << '\n' <<
        "alpha: " << t.alpha << '\n' <<
        "histogram: " << t.histogram << '\n' <<
        "average: " << t.average << '\n' <<
        "multiply: " << t.multiply << '\n' <<
        "alpha_blend: " << t.alpha_blend << '\n' <<
        "mean_color: " << t.mean_color << '\n' <<
        "lighten: " << t.lighten << '\n' <<
        "lighten_top: " << t.lighten_top << '\n' <<
        "lighten_bottom: " << t.lighten_bottom << '\n' <<
        "color_mode:" << color_mode_to_string(t.color_mode) << '\n' <<
        "rotate:" << t.rotate;
    return ostr;
}
