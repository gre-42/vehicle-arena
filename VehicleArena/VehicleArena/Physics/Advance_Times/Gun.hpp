// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Dangling_Unique_Ptr.hpp>
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Memory/Destruction_Guards.hpp>
#include <VehicleArena/Misc/Object.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Physics/Misc/Inventory_Item.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/Scene_Node/IAbsolute_Observer.hpp>
#include <VehicleArena/Stats/Random_Number_Generators.hpp>
#include <functional>
#include <mutex>
#include <optional>

namespace VA {

class RigidBodyVehicle;
class Scene;
class RigidBodies;
class AdvanceTimes;
class SceneNode;
class IPlayer;
class ITeam;
struct BulletProperties;
enum class RigidBodyVehicleFlags;
struct StaticWorld;
template <class TPosition>
struct AudioSourceState;
class BulletGenerator;

class Gun final: public IAbsoluteObserver, public IAdvanceTime, public virtual DanglingBaseClass {
public:
    Gun(AdvanceTimes& advance_times,
        float cool_down,
        RigidBodyVehicle& parent_rb,
        const DanglingBaseClassRef<SceneNode>& node,
        const DanglingBaseClassPtr<SceneNode>& punch_angle_node,
        const BulletProperties& bullet_properties,
        std::function<void(
            const std::optional<VariableAndHash<std::string>>& player,
            const std::string& bullet_suffix,
            const std::optional<VariableAndHash<std::string>>& target,
            const FixedArray<float, 3>& velocity,
            const FixedArray<float, 3>& angular_velocity)> generate_smart_bullet,
        std::function<void(const AudioSourceState<ScenePos>&)> generate_shot_audio,
        const BulletGenerator& bullet_generator,
        std::string ammo_type,
        std::function<FixedArray<float, 3>(bool shooting)> punch_angle_rng,
        std::function<void(const StaticWorld&)> generate_muzzle_flash);
    ~Gun();
    virtual void advance_time(float dt, const StaticWorld& world) override;
    virtual void set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) override;
    void trigger(
        IPlayer* player = nullptr,
        ITeam* team = nullptr);
    const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix() const;
    bool is_none_gun() const;
    const FixedArray<float, 3>& punch_angle() const;
    size_t nbullets_available() const;
    float cool_down() const;
    float bullet_damage() const;
    void set_ypln_node(const DanglingBaseClassRef<SceneNode>& node);
    DanglingBaseClassPtr<SceneNode> get_ypln_node() const;
private:
    bool maybe_generate_bullet(const StaticWorld& world);
    void generate_bullet();
    void generate_muzzle_flash(const StaticWorld& world);
    void generate_shot_audio();
    AdvanceTimes& advance_times_;
    RigidBodyVehicle& parent_rb_;
    DanglingBaseClassPtr<SceneNode> node_;
    DanglingBaseClassPtr<SceneNode> punch_angle_node_;
    DanglingBaseClassPtr<SceneNode> ypln_node_;
    const BulletProperties& bullet_properties_;
    std::function<void(
        const std::optional<VariableAndHash<std::string>>& player,
        const std::string& bullet_suffix,
        const std::optional<VariableAndHash<std::string>>& target,
        const FixedArray<float, 3>& velocity,
        const FixedArray<float, 3>& angular_velocity)> generate_smart_bullet_;
    std::function<void(const AudioSourceState<ScenePos>&)> generate_shot_audio_;
    const BulletGenerator& bullet_generator_;
    InventoryItem ammo_type_;
    bool triggered_;
    IPlayer* player_;
    ITeam* team_;
    float cool_down_;
    float time_since_last_shot_;
    TransformationMatrix<float, ScenePos, 3> absolute_model_matrix_;
    FixedArray<float, 3> punch_angle_;
    std::function<FixedArray<float, 3>(bool shooting)> punch_angle_rng_;
    std::function<void(const StaticWorld&)> generate_muzzle_flash_;
    DestructionGuards dgs_;
    DestructionFunctionsRemovalTokens node_on_clear_;
    std::optional<DestructionFunctionsRemovalTokens> punch_angle_node_on_clear_;
    std::optional<DestructionFunctionsRemovalTokens> ypln_node_on_clear_;
};

}
