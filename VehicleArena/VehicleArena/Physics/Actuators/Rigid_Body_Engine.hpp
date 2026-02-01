// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Physics/Actuators/Engine_Power.hpp>
#include <VehicleArena/Physics/Actuators/Engine_Power_Intent.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Scene_Graph/Status_Writer.hpp>
#include <cstddef>
#include <iosfwd>
#include <memory>
#include <optional>
#include <set>
#include <vector>

namespace VA {

template <class TDir, class TPos, size_t n>
struct RotatingFrame;
template <typename TData, size_t... tshape>
class FixedArray;
class IEngineEventListener;
class EngineEventListeners;
struct EnginePowerDeltaIntent;
struct TirePowerIntent;
enum class VelocityClassification;
struct PhysicsTimeStep;
struct PhysicsPhase;
struct StaticWorld;

class RigidBodyEngine: public StatusWriter {
    friend std::ostream& operator << (std::ostream& ostr, const RigidBodyEngine& engine);

    RigidBodyEngine(const RigidBodyEngine&) = delete;
    RigidBodyEngine& operator = (const RigidBodyEngine&) = delete;
public:
    explicit RigidBodyEngine(
        const std::optional<EnginePower>& engine_power,
        std::shared_ptr<IEngineEventListener> listener);
    ~RigidBodyEngine();

    // StatusWriter
    virtual void write_status(std::ostream& ostr, StatusComponents status_components, const StaticWorld& world) const override;
    virtual float get_value(StatusComponents status_components) const override;
    virtual StatusWriter& child_status_writer(const std::vector<VariableAndHash<std::string>>& name) override;

    // Misc
    float surface_power() const;
    void set_surface_power(const EnginePowerIntent& engine_power_intent);
    TirePowerIntent consume_tire_power(
        size_t tire_id,
        const float* tire_w,
        const EnginePowerDeltaIntent& delta_intent,
        VelocityClassification velocity_classification);
    TirePowerIntent consume_rotor_power(
        size_t rotor_id,
        const float* rotor_w,
        const EnginePowerDeltaIntent& delta_intent);
    void reset_forces();
    void advance_time(
        const PhysicsTimeStep& dt,
        const PhysicsPhase& phase,
        const RotatingFrame<SceneDir, ScenePos, 3>& frame,
        const StaticWorld& static_world);
    float engine_w() const;

private:
    EnginePowerIntent engine_power_intent_;
    std::set<size_t> tires_consumed_;
    std::set<const float*> tires_w_;
    std::optional<EnginePower> engine_power_;
    size_t ntires_old_;
    std::shared_ptr<IEngineEventListener> listener_;
};

std::ostream& operator << (std::ostream& ostr, const TirePowerIntent& tire_power_intent);
std::ostream& operator << (std::ostream& ostr, const RigidBodyEngine& engine);

}
