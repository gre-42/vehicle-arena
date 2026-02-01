// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Geometry/Material/Blend_Distances.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <list>
#include <optional>
#include <string>

namespace VA {

class IImposters;
class ISupplyDepots;
class RigidBodies;
class BulletGenerator;
struct AnimationState;
struct RenderableResourceFilter;
class RenderingResources;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;
class Scene;
template <class T>
class VariableAndHash;

struct RootInstantiationOptions {
    RenderingResources* rendering_resources = nullptr;
    IImposters* imposters = nullptr;
    ISupplyDepots* supply_depots = nullptr;
    AnimationState* animation_state = nullptr;
    RigidBodies* rigid_bodies = nullptr;
    BulletGenerator* bullet_generator = nullptr;
    std::list<VariableAndHash<std::string>>* instantiated_nodes = nullptr;
    const VariableAndHash<std::string>& instance_name;
    const TransformationMatrix<SceneDir, ScenePos, 3>& absolute_model_matrix;
    Scene& scene;
    uint32_t max_imposter_texture_size = 0;
    const RenderableResourceFilter& renderable_resource_filter;
    unsigned int seed = 0;
    std::optional<FixedArray<SceneDir, 3>> surface_normal;
};

}
