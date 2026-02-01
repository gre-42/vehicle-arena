// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Instantiate_Frames.hpp"
#include <VehicleArena/Geometry/Instance/Instance_Information.hpp>
#include <VehicleArena/Geometry/Instance/Rendering_Dynamics.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Scene_Graph/Containers/Scene.hpp>
#include <VehicleArena/Scene_Graph/Elements/Make_Scene_Node.hpp>
#include <VehicleArena/Scene_Graph/Elements/Rendering_Strategies.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <VehicleArena/Scene_Graph/Instantiation/Child_Instantiation_Options.hpp>
#include <VehicleArena/Scene_Graph/Resources/Renderable_Resource_Filter.hpp>
#include <VehicleArena/Scene_Graph/Resources/Scene_Node_Resources.hpp>
#include <algorithm>

using namespace VA;

void VA::instantiate(
    Scene& scene,
    const InstanceInformation<ScenePos>& info,
    SceneNodeResources& scene_node_resources,
    RenderingResources& rendering_resources,
    const std::set<std::string>& required_prefixes,
    const std::set<VariableAndHash<std::string>>& exclude,
    std::set<VariableAndHash<std::string>>* instantiated_resources,
    std::list<VariableAndHash<std::string>>* instantiated_root_nodes)
{
    if (exclude.contains(info.resource_name)) {
        return;
    }
    if (!std::ranges::any_of(required_prefixes, [&](const auto& p){
        return info.resource_name->starts_with(p);
        }))
    {
        return;
    }
    auto name = VariableAndHash{ *info.resource_name + "_inst_" + std::to_string(scene.get_uuid()) };
    auto node = make_unique_scene_node(
        info.trafo.t,
        matrix_2_tait_bryan_angles(info.trafo.R),
        info.scale,
        info.rendering_dynamics == RenderingDynamics::STATIC
            ? PoseInterpolationMode::DISABLED
            : PoseInterpolationMode::ENABLED);
    scene_node_resources.instantiate_child_renderable(
        info.resource_name,
        ChildInstantiationOptions{
            .rendering_resources = &rendering_resources,
            .instance_name = name,
            .scene_node = node.ref(CURRENT_SOURCE_LOCATION),
            .renderable_resource_filter = RenderableResourceFilter{}},
        PreloadBehavior::NO_PRELOAD);
    if (!any(node->rendering_strategies())) {
        lwarn() << "Skipping invisible instance \"" << *name << '"';
    } else {
        scene.auto_add_root_node(name, std::move(node), info.rendering_dynamics);
        if (instantiated_resources != nullptr) {
            instantiated_resources->insert(info.resource_name);
        }
        if (instantiated_root_nodes != nullptr) {
            instantiated_root_nodes->push_back(name);
        }
    }
}
