// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <VehicleArena/Memory/Destruction_Guards.hpp>
#include <VehicleArena/Misc/Object.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Physics/Interfaces/IDamageable.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Scene_Graph/Status_Writer.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <atomic>
#include <functional>
#include <mutex>
#include <string>

namespace VA {

class AdvanceTimes;
class RigidBodyVehicle;
class SceneNode;
class Scene;
class Translator;
template <class TPosition>
struct AudioSourceState;
struct StaticWorld;

struct GenerateExplosion {
    DamageSource damage_sources;
    std::function<void(const AudioSourceState<ScenePos>&, const StaticWorld&)> generate;
};

class DeletingDamageable: public IDamageable, public IAdvanceTime, public StatusWriter, public virtual DanglingBaseClass {
    DeletingDamageable(const DeletingDamageable&) = delete;
    DeletingDamageable& operator = (const DeletingDamageable&) = delete;
public:
    using GenerateExplosions = std::vector<GenerateExplosion>;
    explicit DeletingDamageable(
        Scene& scene,
        AdvanceTimes& advance_times,
        VariableAndHash<std::string> root_node_name,
        float health,
        bool delete_node_when_health_leq_zero,
        std::shared_ptr<Translator> translator,
        GenerateExplosions generate_explosion);
    virtual ~DeletingDamageable() override;
    // IAdvanceTime
    virtual void advance_time(float dt, const StaticWorld& world) override;
    // StatusWriter
    virtual void write_status(std::ostream& ostr, StatusComponents log_components, const StaticWorld& world) const override;
    virtual float get_value(StatusComponents log_components) const override;
    virtual StatusWriter& child_status_writer(const std::vector<VariableAndHash<std::string>>& name) override;
    // IDamageable
    virtual float health() const override;
    virtual void damage(float amount, DamageSource source) override;
protected:
    Scene& scene_;
    AdvanceTimes& advance_times_;
    VariableAndHash<std::string> root_node_name_;
    float health_;
    mutable SafeAtomicRecursiveSharedMutex health_mutex_;
    bool delete_node_when_health_leq_zero_;
    RigidBodyVehicle* rb_;
    std::shared_ptr<Translator> translator_;
    GenerateExplosions generate_explosions_;
    DestructionGuards dgs_;
    DestructionFunctionsRemovalTokens node_on_clear_;
    DestructionFunctionsRemovalTokens rb_on_destroy_;
    DamageSource final_damage_sources_;
    // This field is needed if "delete_node_when_health_leq_zero == false".
    bool explosion_generated_;
};

}
