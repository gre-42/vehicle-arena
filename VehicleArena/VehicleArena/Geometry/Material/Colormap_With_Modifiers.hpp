// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Material/Color_Mode.hpp>
#include <VehicleArena/Geometry/Material/Interpolation_Mode.hpp>
#include <VehicleArena/Geometry/Material/Mipmap_Mode.hpp>
#include <VehicleArena/Geometry/Material/Wrap_Mode.hpp>
#include <VehicleArena/Hashing/Cached_Hash.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <iosfwd>
#include <string>

namespace VA {

struct ColormapWithModifiers {
    VariableAndHash<std::string> filename;
    float desaturate = 0.f;
    float desaturation_exponent = 0.f;
    std::string alpha = "";
    std::string histogram = "";
    std::string average = "";
    std::string multiply = "";
    std::string alpha_blend = "";
    OrderableFixedArray<float, 3> mean_color = { -1.f, -1.f, -1.f };
    OrderableFixedArray<float, 3> lighten = { 0.f, 0.f, 0.f };
    OrderableFixedArray<float, 3> lighten_left = { 0.f, 0.f, 0.f };
    OrderableFixedArray<float, 3> lighten_right = { 0.f, 0.f, 0.f };
    OrderableFixedArray<float, 3> lighten_top = { 0.f, 0.f, 0.f };
    OrderableFixedArray<float, 3> lighten_bottom = { 0.f, 0.f, 0.f };
    OrderableFixedArray<float, 3> color_to_replace = { -1.f, -1.f, -1.f };
    OrderableFixedArray<float, 3> replacement_color = { -1.f, -1.f, -1.f };
    float replacement_tolerance = 0;
    OrderableFixedArray<float, 3> selected_color = { -1.f, -1.f, -1.f };
    float selected_color_near = 0;
    float selected_color_far = INFINITY;
    float edge_sigma = 0.f;
    float times = 1.f;
    float plus = 0.f;
    bool abs = false;
    bool invert = false;
    bool height_to_normals = false;
    bool saturate = false;
    bool multiply_with_alpha = false;
    ColorMode color_mode = ColorMode::UNDEFINED;
    float alpha_fac = 1.f;
    float alpha_exponent = 1.f;
    MipmapMode mipmap_mode = MipmapMode::NO_MIPMAPS;
    InterpolationMode magnifying_interpolation_mode = InterpolationMode::NEAREST;
    InterpolationMode depth_interpolation = InterpolationMode::NEAREST;
    unsigned int anisotropic_filtering_level = 0;
    OrderableFixedArray<WrapMode, 2> wrap_modes = { WrapMode::REPEAT, WrapMode::REPEAT };
    OrderableFixedArray<float, 4> border_color = {1.f, 0.f, 1.f, 1.f};
    int rotate = 0;
    CachedHash hash;
    ColormapWithModifiers& compute_hash();
    std::partial_ordering operator <=> (const ColormapWithModifiers& other) const;
    bool operator == (const ColormapWithModifiers& other) const;
    template <class Archive>
    void serialize(Archive& archive) {
        archive(filename);
        archive(desaturate);
        archive(desaturation_exponent);
        archive(alpha);
        archive(histogram);
        archive(average);
        archive(multiply);
        archive(alpha_blend);
        archive(mean_color);
        archive(lighten);
        archive(lighten_left);
        archive(lighten_right);
        archive(lighten_top);
        archive(lighten_bottom);
        archive(selected_color);
        archive(selected_color_near);
        archive(selected_color_far);
        archive(color_to_replace);
        archive(replacement_color);
        archive(replacement_tolerance);
        archive(edge_sigma);
        archive(times);
        archive(plus);
        archive(abs);
        archive(invert);
        archive(height_to_normals);
        archive(saturate);
        archive(multiply_with_alpha);
        archive(color_mode);
        archive(alpha_fac);
        archive(alpha_exponent);
        archive(mipmap_mode);
        archive(magnifying_interpolation_mode);
        archive(depth_interpolation);
        archive(anisotropic_filtering_level);
        archive(wrap_modes);
        archive(border_color);
        archive(rotate);
        archive(hash);
    }
};

std::ostream& operator << (std::ostream& ostr, const ColormapWithModifiers& t);

}
