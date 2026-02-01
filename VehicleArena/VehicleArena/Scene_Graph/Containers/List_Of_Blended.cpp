// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "List_Of_Blended.hpp"
#include <VehicleArena/Scene_Graph/Elements/Dynamic_Style.hpp>
#include <VehicleArena/Scene_Graph/Elements/Renderable.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/IDynamic_Lights.hpp>
#include <VehicleArena/Scene_Graph/Render_Pass.hpp>

using namespace VA;

ListOfBlended::ListOfBlended() = default;
ListOfBlended::~ListOfBlended() = default;

void ListOfBlended::render(
    IDynamicLights* dynamic_lights,
    const TransformationMatrix<float, ScenePos, 3>& iv,
    const std::list<std::pair<TransformationMatrix<float, ScenePos, 3>, std::shared_ptr<Light>>>& lights,
    const std::list<std::pair<TransformationMatrix<float, ScenePos, 3>, std::shared_ptr<Skidmark>>>& skidmarks,
    const SceneGraphConfig& scene_graph_config,
    const RenderConfig& render_config,
    const RenderPass& render_pass)
{
    list.sort([](Blended& a, Blended& b){ return a.sorting_key() < b.sorting_key(); });
    for (const auto& b : list) {
        DynamicStyle dynamic_style{ dynamic_lights != nullptr
            ? dynamic_lights->get_color(b.m.t)
            : fixed_zeros<float, 3>() };
        b.renderable().render(
            b.mvp,
            b.m,
            iv,
            &dynamic_style,
            lights,
            skidmarks,
            scene_graph_config,
            render_config,
            render_pass,
            b.animation_state.get(),
            b.color_style);
    }
}

ListsOfBlended::ListsOfBlended() = default;
ListsOfBlended::~ListsOfBlended() = default;
