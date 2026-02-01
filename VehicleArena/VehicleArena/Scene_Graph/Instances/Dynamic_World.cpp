// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Dynamic_World.hpp"
#include <VehicleArena/Scene_Graph/Resources/Scene_Node_Resources.hpp>

using namespace VA;

DynamicWorld::DynamicWorld(const SceneNodeResources& scene_node_resources, VariableAndHash<std::string> name)
    : scene_node_resources_{ scene_node_resources }
    , name_{ std::move(name) }
    , inverse_name_{ *name + ".inverse" }
{}

const TransformationMatrix<double, double, 3>* DynamicWorld::get_geographic_mapping() const
{
    return scene_node_resources_.get_geographic_mapping(name_);
}

const TransformationMatrix<double, double, 3>* DynamicWorld::get_inverse_geographic_mapping() const
{
    return scene_node_resources_.get_geographic_mapping(inverse_name_);
}

const FixedScaledUnitVector<float, 3>* DynamicWorld::get_gravity() const
{
    return scene_node_resources_.get_gravity(name_);
}

const FixedScaledUnitVector<float, 3>* DynamicWorld::get_wind() const {
    return scene_node_resources_.get_wind(name_);
}
