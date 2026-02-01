// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>

namespace VA {

struct ParsedResourceName;
struct ResourceInstanceDescriptor;
struct ObjectResourceDescriptor;
class SceneNode;
struct RenderableResourceFilter;
class SceneNodeResources;
class ISupplyDepots;
template <class TPos>
class ColoredVertexArray;
struct ColoredVertexArrayFilter;
struct RootInstantiationOptions;

class BatchResourceInstantiator {
public:
    BatchResourceInstantiator(
        const FixedArray<float, 3>& rotation = fixed_zeros<float, 3>(),
        float scale = 1.f);
    ~BatchResourceInstantiator();

    void add_parsed_resource_name(
        const FixedArray<CompressedScenePos, 3>& p,
        const ParsedResourceName& prn,
        float dyangle,
        float scale);

    void add_parsed_resource_name(
        const FixedArray<CompressedScenePos, 2>& p,
        CompressedScenePos height,
        const ParsedResourceName& prn,
        float dyangle,
        float scale);

    void add_hitbox(
        const VariableAndHash<std::string>& name,
        const ResourceInstanceDescriptor& rid);
    
    void preload(
        const SceneNodeResources& scene_node_resources,
        const RenderableResourceFilter& filter) const;
    
    void instantiate_root_renderables(
        const SceneNodeResources& scene_node_resources,
        const RootInstantiationOptions& options) const;
    
    void instantiate_arrays(
        std::list<std::shared_ptr<ColoredVertexArray<CompressedScenePos>>>& cvas,
        const SceneNodeResources& scene_node_resources,
        const ColoredVertexArrayFilter& filter) const;
        
    void insert_into(std::list<FixedArray<CompressedScenePos, 3>*>& positions);
    void remove(std::set<const FixedArray<CompressedScenePos, 3>*> vertices_to_delete);

    std::list<FixedArray<CompressedScenePos, 3>> hitbox_positions(
        const SceneNodeResources& scene_node_resources) const;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(rotation_);
        archive(scale_);
        archive(object_resource_descriptors_);
        archive(resource_instance_positions_);
        archive(hitboxes_);
    }
private:
    FixedArray<float, 3> rotation_;
    float scale_;
    std::list<ObjectResourceDescriptor> object_resource_descriptors_;
    std::unordered_map<VariableAndHash<std::string>, std::list<ResourceInstanceDescriptor>> resource_instance_positions_;
    std::unordered_map<VariableAndHash<std::string>, std::list<ResourceInstanceDescriptor>> hitboxes_;
};

}
