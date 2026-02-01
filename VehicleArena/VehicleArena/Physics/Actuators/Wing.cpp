// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Wing.hpp"
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Physics/Actuators/Trail_Source.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/ITrail_Extender.hpp>

using namespace VA;

Wing::Wing(
    DanglingBaseClassPtr<SceneNode> angle_of_attack_node,
    DanglingBaseClassPtr<SceneNode> brake_angle_node,
    const TransformationMatrix<float, ScenePos, 3>& relative_location,
    const Interp<float>& fac,
    float lift_coefficient,
    float angle_coefficient_yz,
    float angle_coefficient_zz,
    const FixedArray<float, 3>& drag_coefficients,
    float angle_of_attack,
    float brake_angle,
    std::optional<TrailSource> trail_source)
    : fac{ fac }
    , lift_coefficient{ lift_coefficient }
    , angle_coefficient_yz{ angle_coefficient_yz }
    , angle_coefficient_zz{ angle_coefficient_zz }
    , drag_coefficients{ drag_coefficients }
    , angle_of_attack{ angle_of_attack }
    , brake_angle{ brake_angle }
    , angle_of_attack_movable{ angle_of_attack_node, this->angle_of_attack, relative_location.R.column(0) }
    , brake_angle_movable{ brake_angle_node, this->brake_angle, relative_location.R.column(0) }
    , trail_source{ std::move(trail_source) }
    , relative_location_{ relative_location }
{}

Wing::~Wing() = default;

TransformationMatrix<float, ScenePos, 3> Wing::absolute_location(
    const TransformationMatrix<float, ScenePos, 3>& parent_location) const
{
    return parent_location * relative_location_;
}
