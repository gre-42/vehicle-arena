// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Blended.hpp"
#include <VehicleArena/Scene_Graph/Elements/Renderable.hpp>

using namespace VA;

Blended::Blended(
    std::shared_ptr<const RenderableWithStyle> renderable_with_style,
    const VariableAndHash<std::string>& name,
    const FixedArray<ScenePos, 4, 4>& mvp,
    const TransformationMatrix<float, ScenePos, 3>& m,
    std::shared_ptr<const AnimationState> animation_state,
    const std::list<std::shared_ptr<const ColorStyle>>& color_styles)
    : mvp{ mvp }
    , m{ m }
    , animation_state{ std::move(animation_state) }
    , color_style{ renderable_with_style->style(color_styles, name) }
    , z_order{ (*renderable_with_style)->continuous_blending_z_order() }
    , renderable_with_style_{ std::move(renderable_with_style) }
{ }

Blended::~Blended() = default;
