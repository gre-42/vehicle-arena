// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace VA {

template <class T>
class DanglingBaseClassRef;
template <typename TData, size_t... tshape>
class FixedArray;
class Scene;
class SceneNode;
class SceneNodeResources;
class RenderingResources;
class IParticleRenderer;
class RigidBodies;
class BulletGenerator;
template <class T>
class VariableAndHash;
struct StaticWorld;

enum class ParticleContainer {
    PHYSICS,
    NODE,
    INSTANCE
};

void from_json(const nlohmann::json& j, ParticleContainer& pc);

class SmokeParticleGenerator {
public:
    SmokeParticleGenerator(
        RenderingResources& rendering_resources,
        SceneNodeResources& scene_node_resources,
        std::shared_ptr<IParticleRenderer> particle_renderer,
        Scene& scene,
        RigidBodies& rigid_bodies);
    void generate_root(
        const VariableAndHash<std::string>& resource_name,
        const VariableAndHash<std::string>& node_name,
        const FixedArray<ScenePos, 3>& position,
        const FixedArray<float, 3>& rotation,
        const FixedArray<float, 3>& velocity,
        float air_resistance_halflife,
        float texture_layer,
        float animation_duration,
        ParticleContainer particle_container,
        const StaticWorld& static_world);
    void generate_instance(
        const VariableAndHash<std::string>& resource_name,
        const FixedArray<ScenePos, 3>& position,
        const FixedArray<float, 3>& rotation,
        const FixedArray<float, 3>& velocity,
        float air_resistance_halflife,
        float texture_layer,
        const StaticWorld& static_world);
    void generate_physics_node(
        const VariableAndHash<std::string>& resource_name,
        const FixedArray<ScenePos, 3>& position,
        const FixedArray<float, 3>& rotation,
        float animation_duration,
        const StaticWorld& static_world);
    void generate_root_node(
        const VariableAndHash<std::string>& resource_name,
        const VariableAndHash<std::string>& node_name,
        const FixedArray<ScenePos, 3>& position,
        const FixedArray<float, 3>& rotation,
        const FixedArray<float, 3>& velocity,
        float air_resistance_halflife,
        float animation_duration,
        const StaticWorld& static_world);
    void generate_child_node(
        DanglingBaseClassRef<SceneNode> parent,
        const VariableAndHash<std::string>& resource_name,
        const VariableAndHash<std::string>& child_node_name,
        const FixedArray<ScenePos, 3>& relative_position,
        float animation_duration);
    std::string generate_suffix();
    void set_bullet_generator(BulletGenerator& bullet_generator);
private:
    RenderingResources& rendering_resources_;
    SceneNodeResources& scene_node_resources_;
    std::shared_ptr<IParticleRenderer> particle_renderer_;
    Scene& scene_;
    RigidBodies& rigid_bodies_;
    BulletGenerator* bullet_generator_;
};

}
