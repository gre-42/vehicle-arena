// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Physics/Physics_Engine/Penetration_Limits_Factory.hpp>
#include <memory>

namespace VA {

template <class TDir, class TPos, size_t n>
class TransformationMatrix;
class RigidBodyPulses;
class RigidBodyVehicle;
class RigidBodies;
template <class T>
class DeleteFromPool;

// Source: https://en.wikipedia.org/wiki/List_of_moments_of_inertia
RigidBodyPulses rigid_cuboid_pulses(
    float mass,
    const FixedArray<float, 3>& size,
    const FixedArray<float, 3>& com = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& v = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& w = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& I_rotation = fixed_zeros<float, 3>(),
    const PenetrationLimitsFactory& pl = PenetrationLimitsFactory::inf());

// Source: https://en.wikipedia.org/wiki/List_of_moments_of_inertia
RigidBodyPulses rigid_disk_pulses(
    float mass,
    float radius,
    const FixedArray<float, 3>& com = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& v = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& w = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& I_rotation = fixed_zeros<float, 3>(),
    const PenetrationLimitsFactory& pl = PenetrationLimitsFactory::inf());

std::unique_ptr<RigidBodyVehicle, DeleteFromPool<RigidBodyVehicle>> rigid_cuboid(
    std::string name,
    std::string asset_id,
    float mass,
    const FixedArray<float, 3>& size,
    const FixedArray<float, 3>& com = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& v = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& w = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& I_rotation = fixed_zeros<float, 3>(),
    const PenetrationLimitsFactory& pl = PenetrationLimitsFactory::inf(),
    const TransformationMatrix<double, double, 3>* geographic_coordinates = nullptr);

std::unique_ptr<RigidBodyVehicle, DeleteFromPool<RigidBodyVehicle>> rigid_disk(
    std::string name,
    std::string asset_id,
    float mass,
    float radius,
    const FixedArray<float, 3>& com = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& v = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& w = fixed_zeros<float, 3>(),
    const FixedArray<float, 3>& I_rotation = fixed_zeros<float, 3>(),
    const PenetrationLimitsFactory& pl = PenetrationLimitsFactory::inf(),
    const TransformationMatrix<double, double, 3>* geographic_coordinates = nullptr);

}
