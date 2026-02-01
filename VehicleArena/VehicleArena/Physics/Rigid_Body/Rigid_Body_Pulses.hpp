// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Misc/Source_Location.hpp>
#include <VehicleArena/Physics/Physics_Engine/Penetration_Limits_Factory.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <iosfwd>

namespace VA {

template <class TDir, class TPos, size_t tsize>
struct VectorAtPosition;
class RigidBodyPulses;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TDir, class TPos, size_t n>
struct RotatingFrame;

std::ostream& operator << (std::ostream& ostr, const RigidBodyPulses& rbi);

class RigidBodyPulses {
    friend std::ostream& VA::operator << (std::ostream& ostr, const RigidBodyPulses& rbi);
public:
    RigidBodyPulses(
        float mass,
        const FixedArray<float, 3, 3>& I,   // inertia tensor
        const FixedArray<float, 3>& com,    // center of mass
        const FixedArray<float, 3>& v_com,  // velocity of the center of mass
        const FixedArray<float, 3>& w,      // angular velocity
        const FixedArray<ScenePos, 3>& position,
        const FixedArray<float, 3>& rotation,
        bool I_is_diagonal,
        const PenetrationLimitsFactory& penetration_limits);

    FixedArray<float, 3> abs_z() const;
    FixedArray<ScenePos, 3> abs_position() const;
    TransformationMatrix<float, ScenePos, 3> abs_transformation() const;
    const FixedArray<float, 3, 3>& abs_I() const;
    const FixedArray<float, 3, 3>& abs_I_inv() const;
    RotatingFrame<SceneDir, ScenePos, 3> rotating_frame() const;
    FixedArray<float, 3> velocity() const;
    FixedArray<float, 3> velocity_at_position(const FixedArray<ScenePos, 3>& position) const;
    FixedArray<float, 3> solve_abs_I(const FixedArray<float, 3>& x) const;
    FixedArray<float, 3> dot1d_abs_I(const FixedArray<float, 3>& x) const;
    FixedArray<ScenePos, 3> transform_to_world_coordinates(const FixedArray<float, 3>& v) const;
    void set_pose(const FixedArray<float, 3, 3>& rotation, const FixedArray<ScenePos, 3>& position);
    void integrate_delta_v(const FixedArray<float, 3>& dv, float dt);
    void integrate_delta_angular_momentum(const FixedArray<float, 3>& dL, float extra_w, float dt);
    void integrate_impulse(const VectorAtPosition<float, ScenePos, 3>& J, float extra_w, float dt, const SourceLocation& loc);
    float energy() const;
    float effective_mass(const VectorAtPosition<float, ScenePos, 3>& vp) const;

    void advance_time(float dt);

    float mass_;
    FixedArray<float, 3, 3> I_;     // inertia tensor
    FixedArray<float, 3> com_;      // center of mass
    FixedArray<float, 3> v_com_;    // velocity of the center of mass
    FixedArray<float, 3> w_;        // angular velocity

    FixedArray<float, 3, 3> rotation_;
    FixedArray<ScenePos, 3> abs_com_;
    
    PenetrationLimitsFactory penetration_limits_;
    SourceLocation last_update_source_location_;
private:
    bool I_is_diagonal_;
    void update_abs_I_and_inv();
    FixedArray<float, 3, 3> abs_I_;
    FixedArray<float, 3, 3> abs_I_inv_;
#ifndef NDEBUG
    mutable FixedArray<float, 3, 3> abs_I_rotation_;
#endif
};

}
