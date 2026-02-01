// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Initialization/Default_Uninitialized_Vector.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <chrono>
#include <cstdint>
#include <list>
#include <optional>
#include <string>
#include <vector>

namespace VA {

template <class T>
class VariableAndHash;
template <class T>
class DanglingBaseClassPtr;
class SceneNode;
struct TrackElement;
class RigidBodyVehicle;
enum class RaceState;
template <typename TData, size_t... tshape>
class FixedArray;
class DestructionFunctions;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;
class VehicleSpawner;
class SceneVehicle;

enum class SelectNextVehicleQuery: uint8_t {
    NONE = 0,
    ENTER_IF_FREE = 1 << 0,
    EXIT = 1 << 1,
    ENTER_BY_FORCE = 1 << 2,
    ANY_ENTER = ENTER_IF_FREE | ENTER_BY_FORCE
};

inline bool any(SelectNextVehicleQuery q) {
    return q != SelectNextVehicleQuery::NONE;
}

inline SelectNextVehicleQuery operator & (SelectNextVehicleQuery a, SelectNextVehicleQuery b) {
    return (SelectNextVehicleQuery)((int)a & (int)b);
}

inline SelectNextVehicleQuery operator | (SelectNextVehicleQuery a, SelectNextVehicleQuery b) {
    return (SelectNextVehicleQuery)((int)a | (int)b);
}

class IPlayer {
public:
    virtual const VariableAndHash<std::string>& id() const = 0;
    virtual std::string title() const = 0;
    virtual std::optional<VariableAndHash<std::string>> target_id() const = 0;
    virtual bool reset_vehicle_requested() = 0;
    virtual bool can_reset_vehicle(
        const TransformationMatrix<SceneDir, ScenePos, 3>& trafo) const = 0;
    virtual bool try_reset_vehicle(
        const TransformationMatrix<SceneDir, ScenePos, 3>& trafo) = 0;
    virtual void select_next_vehicle(
        std::chrono::steady_clock::time_point time,
        SelectNextVehicleQuery q,
        const std::string& seat) = 0;
    virtual void set_next_vehicle(
        VehicleSpawner& spawner,
        SceneVehicle& vehicle,
        const std::string& seat) = 0;
    virtual void clear_next_vehicle() = 0;
    virtual std::vector<DanglingBaseClassPtr<SceneNode>> moving_nodes() const = 0;
    virtual void notify_race_started() = 0;
    virtual RaceState notify_lap_finished(
        float race_time_seconds,
        const std::string& asset_id,
        const UUVector<FixedArray<float, 3>>& vehicle_colors,
        const std::list<float>& lap_times_seconds,
        const std::list<TrackElement>& track) = 0;
    virtual void notify_kill(RigidBodyVehicle& rigid_body_vehicle) = 0;
    virtual void notify_bullet_generated(std::chrono::steady_clock::time_point time) = 0;
    virtual DestructionFunctions& on_destroy_player() = 0;
    virtual DestructionFunctions& on_clear_vehicle() = 0;
};

}
