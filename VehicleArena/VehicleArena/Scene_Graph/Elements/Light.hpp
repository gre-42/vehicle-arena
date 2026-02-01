// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Texture/ITexture_Handle.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <memory>
#include <optional>

namespace VA {

enum class ExternalRenderPassType;

struct Light {
    FixedArray<float, 3> ambient{1.f, 1.f, 1.f};
    FixedArray<float, 3> diffuse{1.f, 1.f, 1.f};
    FixedArray<float, 3> specular{1.f, 1.f, 1.f};
    FixedArray<float, 3> fresnel_ambient{1.f, 1.f, 1.f};
    FixedArray<float, 3> fog_ambient{1.f, 1.f, 1.f};
    std::shared_ptr<ITextureHandle> lightmap_color;
    std::shared_ptr<ITextureHandle> lightmap_depth;
    std::optional<FixedArray<ScenePos, 4, 4>> vp;
    ExternalRenderPassType shadow_render_pass;
    bool emits_colors() const;
    size_t shading_hash() const;
};

}
