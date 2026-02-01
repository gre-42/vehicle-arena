// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Billboard_Id.hpp>
#include <VehicleArena/Physics/Misc/Inventory_Item.hpp>
#include <cstdint>
#include <list>
#include <string>
#include <unordered_map>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
struct ResourceInstanceDescriptor;
struct ObjectResourceDescriptor;
class SceneNodeResources;
enum class AggregateMode;

struct ParsedResourceName {
    VariableAndHash<std::string> name;
    BillboardId billboard_id;
    float yangle;
    float probability;
    float probability1;
    float min_distance_to_bdry;
    float max_distance_to_bdry;
    AggregateMode aggregate_mode;
    bool create_imposter;
    uint32_t max_imposter_texture_size;
    VariableAndHash<std::string> hitbox;
    std::unordered_map<InventoryItem, uint32_t> supplies;
    float supplies_cooldown;
    template <class Archive>
    void serialize(Archive& archive) {
        archive(name);
        archive(billboard_id);
        archive(yangle);
        archive(probability);
        archive(probability1);
        archive(min_distance_to_bdry);
        archive(max_distance_to_bdry);
        archive(aggregate_mode);
        archive(create_imposter);
        archive(max_imposter_texture_size);
        archive(hitbox);
        archive(supplies);
        archive(supplies_cooldown);
    }
};

ParsedResourceName parse_resource_name(
    const SceneNodeResources& resources,
    const std::string& name);

}
