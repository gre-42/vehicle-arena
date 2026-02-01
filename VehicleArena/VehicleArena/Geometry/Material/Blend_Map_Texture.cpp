// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Blend_Map_Texture.hpp"
#include <VehicleArena/Math/Orderable_Fixed_Array_Hash.hpp>
#include <stdexcept>

using namespace std::string_view_literals;
using namespace VA;

BlendMapRole VA::blend_map_role_from_string(std::string_view s) {
    static const std::unordered_map<std::string_view, BlendMapRole> m{
        {"summand"sv, BlendMapRole::SUMMAND},
        {"detail_base"sv, BlendMapRole::DETAIL_BASE},
        {"detail_mask_r"sv, BlendMapRole::DETAIL_MASK_R},
        {"detail_mask_g"sv, BlendMapRole::DETAIL_MASK_G},
        {"detail_mask_b"sv, BlendMapRole::DETAIL_MASK_B},
        {"detail_mask_a"sv, BlendMapRole::DETAIL_MASK_A},
        {"detail_color"sv, BlendMapRole::DETAIL_COLOR}};
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown blend map role: \"" + std::string{ s } + '"');
    }
    return it->second;
}

BlendMapUvSource VA::blend_map_uv_source_from_string(std::string_view s) {
    static_assert(BlendMapUvSource::VERTICAL_LAST == BlendMapUvSource::VERTICAL4);
    static const std::unordered_map<std::string_view, BlendMapUvSource> m{
        {"vertical0"sv, BlendMapUvSource::VERTICAL0},
        {"vertical1"sv, BlendMapUvSource::VERTICAL1},
        {"vertical2"sv, BlendMapUvSource::VERTICAL2},
        {"vertical3"sv, BlendMapUvSource::VERTICAL3},
        {"vertical4"sv, BlendMapUvSource::VERTICAL4},
        {"horizontal_xz"sv, BlendMapUvSource::HORIZONTAL_XZ},
        {"horizontal_xy"sv, BlendMapUvSource::HORIZONTAL_XY}
    };
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown UV source: \"" + std::string{ s } + '"');
    }
    return it->second;
}

BlendMapReductionOperation VA::blend_map_reduction_operation_from_string(std::string_view s) {
    static const std::unordered_map<std::string_view, BlendMapReductionOperation> m{
        {"plus"sv, BlendMapReductionOperation::PLUS},
        {"minus"sv, BlendMapReductionOperation::MINUS},
        {"times"sv, BlendMapReductionOperation::TIMES},
        {"feather"sv, BlendMapReductionOperation::FEATHER},
        {"invert"sv, BlendMapReductionOperation::INVERT},
        {"blend"sv, BlendMapReductionOperation::BLEND},
        {"replace_color"sv, BlendMapReductionOperation::REPLACE_COLOR},
        {"colorize"sv, BlendMapReductionOperation::COLORIZE}
    };
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown blend map reduction operation: \"" + std::string{ s } + '"');
    }
    return it->second;
}

BlendMapReweightMode VA::blend_map_reweight_mode_from_string(std::string_view s) {
    static const std::unordered_map<std::string_view, BlendMapReweightMode> m{
        {"undefined"sv, BlendMapReweightMode::UNDEFINED},
        {"enabled"sv, BlendMapReweightMode::ENABLED},
        {"disabled"sv, BlendMapReweightMode::DISABLED}
    };
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown blend map reweight mode: \"" + std::string{ s } + '"');
    }
    return it->second;
}

size_t BlendMapTexture::modifiers_hash() const {
    return hash_combine(
        texture_descriptor.color.filename->empty(),
        texture_descriptor.specular.filename->empty(),
        texture_descriptor.normal.filename->empty(),
        texture_descriptor.color.color_mode,
        texture_descriptor.specular.color_mode,
        texture_descriptor.normal.color_mode,
        min_height,
        max_height,
        distances,
        normal,
        cosines,
        discreteness,
        offset,
        scale,
        weight,
        cweight_id,
        plus,
        min_detail_weight,
        role,
        uv_source,
        reduction,
        reweight_mode);
}
