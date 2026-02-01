// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <cstddef>
#include <list>
#include <optional>

namespace VA {

struct WayPoint;
template <typename TData, size_t... tshape>
class FixedArray;
template <class TPoint, class TFlags>
struct PointAndFlags;
enum class WayPointLocation;

class AiWaypoint {
public:
    using WayPoint = PointAndFlags<FixedArray<CompressedScenePos, 3>, WayPointLocation>;

    AiWaypoint(
        const std::optional<WayPoint>& position_of_destination,
        const std::optional<FixedArray<float, 3>>& velocity_of_destination,
        const std::optional<FixedArray<float, 3>>& velocity_at_destination,
        const std::list<WayPoint>* waypoint_history);
    bool has_position_of_destination() const;
    FixedArray<CompressedScenePos, 3> position_of_destination(CompressedScenePos dy) const;
    WayPointLocation flags() const;
    WayPointLocation latest_history_flags() const;
    bool has_velocity_at_destination() const;
    FixedArray<float, 3> velocity_at_destination() const;
    FixedArray<float, 3> velocity_of_destination(const FixedArray<float, 3>& deflt) const;
    FixedArray<float, 3> velocity_at_destination(const FixedArray<float, 3>& deflt) const;
    FixedArray<CompressedScenePos, 3> interpolated_position(
        const FixedArray<CompressedScenePos, 3>& vehicle_position,
        ScenePos radius_squared,
        CompressedScenePos dy) const;

private:
    const std::optional<WayPoint>& position_of_destination_;
    const std::optional<FixedArray<float, 3>>& velocity_of_destination_;
    const std::optional<FixedArray<float, 3>>& velocity_at_destination_;
    const std::list<WayPoint>* waypoint_history_;
};

}
