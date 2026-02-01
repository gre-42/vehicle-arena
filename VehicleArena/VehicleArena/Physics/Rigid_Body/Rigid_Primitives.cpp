// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Rigid_Primitives.hpp"
#include <VehicleArena/Geometry/Fixed_Cross.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

using namespace VA;

FixedArray<float, 3, 3> permuted_diagonal_matrix(
    const FixedArray<float, 3, 3>& I,
    const FixedArray<float, 3>& rotation)
{
    if (all(rotation == 0.f)) {
        return I;
    }
    if (any(abs(I) == INFINITY)) {
        throw std::runtime_error("INF not supported yet for permuted matrices");
    }
    auto R = tait_bryan_angles_2_matrix(rotation);
    auto Rr = R.applied([](float v){ return std::round(v); });
    if (any(abs(Rr - R) > 1e-12f)) {
        throw std::runtime_error("Rotation cannot be rounded without error");
    }
    auto m = dot2d(R, dot2d(I, R.T()));
    bool is_diagonal = all(abs((1.f - fixed_identity_array<float, 3>()) * m) == 0.f);
    if (!is_diagonal) {
        throw std::runtime_error("Permuted matrix is not diagonal");
    }
    return m;
}

RigidBodyPulses VA::rigid_cuboid_pulses(
    float mass,
    const FixedArray<float, 3>& size,
    const FixedArray<float, 3>& com,
    const FixedArray<float, 3>& v,
    const FixedArray<float, 3>& w,
    const FixedArray<float, 3>& I_rotation,
    const PenetrationLimitsFactory& pl)
{
    // From: https://en.wikipedia.org/wiki/List_of_moments_of_inertia
    auto I = FixedArray<float, 3, 3>::init(
        1.f / 12.f * mass * (squared(size(1)) + squared(size(2))), 0.f, 0.f,
        0.f, 1.f / 12.f * mass * (squared(size(0)) + squared(size(2))), 0.f,
        0.f, 0.f, 1.f / 12.f * mass * (squared(size(0)) + squared(size(1))));

    return RigidBodyPulses{
        mass,
        permuted_diagonal_matrix(I, I_rotation),    // I
        com,                                        // com
        v,                                          // v
        w,                                          // w
        fixed_nans<ScenePos, 3>(),                  // position
        fixed_zeros<float, 3>(),                    // rotation (not NAN to pass rogridues angle assertion)
        true,                                       // I_is_diagonal
        pl
    };
}

RigidBodyPulses VA::rigid_disk_pulses(
    float mass,
    float radius,
    const FixedArray<float, 3>& com,
    const FixedArray<float, 3>& v,
    const FixedArray<float, 3>& w,
    const FixedArray<float, 3>& I_rotation,
    const PenetrationLimitsFactory& pl)
{
    // From: https://en.wikipedia.org/wiki/List_of_moments_of_inertia
    auto I = FixedArray<float, 3, 3>::init(
        1.f / 4.f * mass * squared(radius), 0.f, 0.f,
        0.f, 1.f / 4.f * mass * squared(radius), 0.f,
        0.f, 0.f, 1.f / 2.f * mass * squared(radius));

    return RigidBodyPulses{
        mass,
        permuted_diagonal_matrix(I, I_rotation),    // I
        com,                                        // com
        v,                                          // v
        w,                                          // w
        fixed_nans<ScenePos, 3>(),                  // position
        fixed_zeros<float, 3>(),                    // rotation (not NAN to pass rogridues angle assertion)
        true,                                       // I_is_diagonal
        pl,
    };
}

std::unique_ptr<RigidBodyVehicle, DeleteFromPool<RigidBodyVehicle>> VA::rigid_cuboid(
    std::string name,
    std::string asset_id,
    float mass,
    const FixedArray<float, 3>& size,
    const FixedArray<float, 3>& com,
    const FixedArray<float, 3>& v,
    const FixedArray<float, 3>& w,
    const FixedArray<float, 3>& I_rotation,
    const PenetrationLimitsFactory& pl,
    const TransformationMatrix<double, double, 3>* geographic_coordinates)
{
    return global_object_pool.create_unique<RigidBodyVehicle>(
        CURRENT_SOURCE_LOCATION,
        rigid_cuboid_pulses(mass, size, com, v, w, I_rotation, pl),
        std::move(name),
        std::move(asset_id),
        geographic_coordinates);
}

std::unique_ptr<RigidBodyVehicle, DeleteFromPool<RigidBodyVehicle>> VA::rigid_disk(
    std::string name,
    std::string asset_id,
    float mass,
    float radius,
    const FixedArray<float, 3>& com,
    const FixedArray<float, 3>& v,
    const FixedArray<float, 3>& w,
    const FixedArray<float, 3>& I_rotation,
    const PenetrationLimitsFactory& pl,
    const TransformationMatrix<double, double, 3>* geographic_coordinates)
{
    return global_object_pool.create_unique<RigidBodyVehicle>(
        CURRENT_SOURCE_LOCATION,
        rigid_disk_pulses(mass, radius, com, v, w, I_rotation, pl),
        std::move(name),
        std::move(asset_id),
        geographic_coordinates);
}
