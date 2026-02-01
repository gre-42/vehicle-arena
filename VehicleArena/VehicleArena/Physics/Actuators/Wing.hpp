// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Interp.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Actuators/Trail_Source.hpp>
#include <VehicleArena/Physics/Actuators/Wing_Angle.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <optional>

namespace VA {

class ITrailExtender;

class Wing {
    Wing(const Wing&) = delete;
    Wing& operator = (const Wing&) = delete;
public:
    Wing(
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
        std::optional<TrailSource> trail_source);
    ~Wing();
    TransformationMatrix<float, ScenePos, 3> absolute_location(
        const TransformationMatrix<float, ScenePos, 3>& parent_location) const;
    Interp<float> fac;
    float lift_coefficient;
    float angle_coefficient_yz;
    float angle_coefficient_zz;
    FixedArray<float, 3> drag_coefficients;
    float angle_of_attack;
    float brake_angle;
    WingAngle angle_of_attack_movable;
    WingAngle brake_angle_movable;
    std::optional<TrailSource> trail_source;

private:
    TransformationMatrix<float, ScenePos, 3> relative_location_;
};

}
