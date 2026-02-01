// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Ai_Waypoint.hpp"
#include <VehicleArena/Geometry/Graph/Point_And_Flags.hpp>
#include <VehicleArena/Geometry/Primitives/Ray_Segment_3D.hpp>
#include <VehicleArena/Geometry/Primitives/Ray_Sphere_Intersection.hpp>
#include <VehicleArena/Scene_Graph/Way_Point_Location.hpp>
#include <stdexcept>

using namespace VA;

AiWaypoint::AiWaypoint(
    const std::optional<WayPoint>& position_of_destination,
    const std::optional<FixedArray<float, 3>>& velocity_of_destination,
    const std::optional<FixedArray<float, 3>>& velocity_at_destination,
    const std::list<WayPoint>* waypoint_history)
    : position_of_destination_{ position_of_destination }
    , velocity_of_destination_{ velocity_of_destination }
    , velocity_at_destination_{ velocity_at_destination }
    , waypoint_history_{ waypoint_history }
{}

FixedArray<CompressedScenePos, 3> AiWaypoint::interpolated_position(
    const FixedArray<CompressedScenePos, 3>& vehicle_position,
    ScenePos radius_squared,
    CompressedScenePos dy) const
{
    if (!position_of_destination_.has_value()) {
        throw std::runtime_error("Position of desination is undefined");
    }
    auto& pod = position_of_destination_->position;
    FixedArray<CompressedScenePos, 3> dy3{ (CompressedScenePos)0.f, dy, (CompressedScenePos)0.f };
    if ((waypoint_history_ == nullptr) ||
        waypoint_history_->empty())
    {
        return pod + dy3;
    }
    RaySegment3D<ScenePos, CompressedScenePos> rs(waypoint_history_->rbegin()->position, pod);
    ScenePos lambda;
    if (!ray_intersects_sphere(
        funpack(rs.start),
        funpack(rs.direction),
        funpack(vehicle_position),
        radius_squared,
        &lambda,
        (ScenePos*)nullptr))
    {
        return pod + dy3;
    } else {
        if ((lambda >= 0) && (lambda < rs.length)) {
            return rs.start + (rs.direction * lambda).casted<CompressedScenePos>() + dy3;
        } else {
            return pod + dy3;
        }
    }
    return pod + dy3;
}

bool AiWaypoint::has_position_of_destination() const {
    return position_of_destination_.has_value();
}

FixedArray<CompressedScenePos, 3> AiWaypoint::position_of_destination(CompressedScenePos dy) const {
    if (!position_of_destination_.has_value()) {
        throw std::runtime_error("Position of desintation not defined");
    }
    auto res = position_of_destination_->position;
    res(1) += dy;
    return res;
}

WayPointLocation AiWaypoint::flags() const {
    if (!position_of_destination_.has_value()) {
        throw std::runtime_error("Position of desintation not defined");
    }
    return position_of_destination_->flags;
}

FixedArray<float, 3> AiWaypoint::velocity_of_destination(const FixedArray<float, 3>& deflt) const {
    return velocity_of_destination_.value_or(deflt);
}

FixedArray<float, 3> AiWaypoint::velocity_at_destination(const FixedArray<float, 3>& deflt) const {
    return velocity_at_destination_.value_or(deflt);
}

WayPointLocation AiWaypoint::latest_history_flags() const {
    if (waypoint_history_ == nullptr) {
        throw std::runtime_error("Waypoint history is null");
    }
    if (waypoint_history_->empty()) {
        return WayPointLocation::NONE;
    }
    return waypoint_history_->rbegin()->flags;
}

bool AiWaypoint::has_velocity_at_destination() const {
    return velocity_at_destination_.has_value();
}

FixedArray<float, 3> AiWaypoint::velocity_at_destination() const {
    if (!velocity_at_destination_.has_value()) {
        throw std::runtime_error("Velocity at destination is undefined");
    }
    return *velocity_at_destination_;
}
