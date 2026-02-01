// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Graph/Interfaces/IScene_Node_Resource.hpp>
#include <string>
#include <vector>

namespace VA {

template <class T>
class VariableAndHash;
class SceneNodeResources;

class CompoundResource: public ISceneNodeResource {
public:
    CompoundResource(
        SceneNodeResources& scene_node_resources,
        std::vector<VariableAndHash<std::string>> resource_names);
    ~CompoundResource();
    
    // Misc
    virtual void preload(const RenderableResourceFilter& filter) const override;
    virtual void instantiate_child_renderable(const ChildInstantiationOptions& options) const override;
    virtual void instantiate_root_renderables(const RootInstantiationOptions& options) const override;
    virtual TransformationMatrix<double, double, 3> get_geographic_mapping(const TransformationMatrix<double, double, 3>& absolute_model_matrix) const override;
    virtual AggregateMode get_aggregate_mode() const override;
    virtual std::list<SpawnPoint> get_spawn_points() const override;
    virtual WayPointSandboxes get_way_points() const override;

    // Output
    virtual void save_to_obj_file(
        const std::string& prefix,
        const TransformationMatrix<float, ScenePos, 3>* model_matrix) const override;

    // Animation
    virtual std::shared_ptr<AnimatedColoredVertexArrays> get_arrays(const ColoredVertexArrayFilter& filter) const override;
    virtual std::list<std::shared_ptr<AnimatedColoredVertexArrays>> get_rendering_arrays() const override;
    virtual std::list<TypedMesh<std::shared_ptr<IIntersectable>>> get_intersectables() const override;

    // Modifiers
    virtual void modify_physics_material_tags(
        PhysicsMaterial add,
        PhysicsMaterial remove,
        const ColoredVertexArrayFilter& filter) override;
    virtual void generate_instances() override;
    virtual void create_barrier_triangle_hitboxes(
        float depth,
        PhysicsMaterial destination_physics_material,
        const ColoredVertexArrayFilter& filter) override;

    // Transformations
    virtual std::shared_ptr<ISceneNodeResource> generate_grind_lines(
        float edge_angle,
        float averaged_normal_angle,
        const ColoredVertexArrayFilter& filter) const override;
private:
    SceneNodeResources& scene_node_resources_;
    std::vector<VariableAndHash<std::string>> resource_names_;
};

}
