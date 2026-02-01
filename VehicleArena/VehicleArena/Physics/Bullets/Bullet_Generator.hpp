// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>

namespace VA {

template <class T>
class VariableAndHash;
class DynamicWorld;
enum class AggregateMode;
enum class PhysicsMaterial: uint32_t;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <typename TData, size_t... tshape>
class FixedArray;
class RenderingResources;
class SceneNodeResources;
class SmokeParticleGenerator;
class DynamicLights;
class ITrailRenderer;
class RigidBodyVehicle;
class Scene;
class RigidBodies;
class AdvanceTimes;
class SceneNode;
class IPlayer;
class ITeam;
struct BulletExplosion;
struct BulletProperties;
enum class RigidBodyVehicleFlags;
template <class TPosition>
struct AudioSourceState;
class BulletPropertyDb;
class RenderingResources;
struct RenderableResourceFilter;

using UpdateAudioSourceState = std::function<void(const AudioSourceState<ScenePos>*)>;
using GenerateSmartBullet = std::function<void(
    const std::optional<VariableAndHash<std::string>>& player,
    const std::string& bullet_suffix,
    const std::optional<VariableAndHash<std::string>>& target,
    const FixedArray<float, 3>& velocity,
    const FixedArray<float, 3>& angular_velocity)>;

class BulletGenerator{
public:
    BulletGenerator(
        RenderingResources* rendering_resources,
        Scene& scene,
        SceneNodeResources& scene_node_resources,
        SmokeParticleGenerator& smoke_generator,
        DynamicLights& dynamic_lights,
        RigidBodies& rigid_bodies,
        AdvanceTimes& advance_times,
        ITrailRenderer& trail_renderer,
        const DynamicWorld& dynamic_world,
        std::function<void(
            const AudioSourceState<ScenePos>& state,
            const VariableAndHash<std::string>& audio_resource)> generate_bullet_explosion_audio,
        std::function<UpdateAudioSourceState(
            const AudioSourceState<ScenePos>& state,
            const VariableAndHash<std::string>& audio_resource)> generate_bullet_engine_audio);
    void generate_bullet(
        const BulletProperties& bullet_properties,
        const GenerateSmartBullet& generate_smart_bullet,
        RigidBodyVehicle* non_collider,
        const TransformationMatrix<SceneDir, ScenePos, 3>& location,
        const FixedArray<SceneDir, 3>& velocity,
        const FixedArray<SceneDir, 3>& angular_velocity,
        IPlayer* player,
        ITeam* team) const;
    void preload(
        const BulletProperties& bullet_properties,
        const RenderableResourceFilter& filter) const;
    AggregateMode get_aggregate_mode(const BulletProperties& bullet_properties) const;
    PhysicsMaterial get_physics_material(const BulletProperties& bullet_properties) const;
private:
    RenderingResources* rendering_resources_;
    Scene& scene_;
    SceneNodeResources& scene_node_resources_;
    SmokeParticleGenerator& smoke_generator_;
    DynamicLights& dynamic_lights_;
    RigidBodies& rigid_bodies_;
    AdvanceTimes& advance_times_;
    ITrailRenderer& trail_renderer_;
    const DynamicWorld& dynamic_world_;
    std::function<void(
        const AudioSourceState<ScenePos>& state,
        const VariableAndHash<std::string>& audio_resource)> generate_bullet_explosion_audio_;
    std::function<UpdateAudioSourceState(
        const AudioSourceState<ScenePos>& state,
        const VariableAndHash<std::string>& audio_resource)> generate_bullet_engine_audio_;
};

}
