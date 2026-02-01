// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Surface_Contact_Db.hpp"
#include <VehicleArena/Geometry/Physics_Material.hpp>
#include <VehicleArena/Physics/Collision/Record/Intersection_Scene.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Surface_Contact_Info.hpp>
#include <stdexcept>

using namespace VA;

SurfaceContactDb::SurfaceContactDb() = default;

SurfaceContactDb::~SurfaceContactDb() = default;

void SurfaceContactDb::store_contact_info(
    SurfaceContactInfo info,
    PhysicsMaterial material0,
    PhysicsMaterial material1)
{
    if ((material0 & PhysicsMaterial::SURFACE_MASK) != material0) {
        throw std::runtime_error("Material 0 has modifiers");
    }
    if ((material1 & PhysicsMaterial::SURFACE_MASK) != material1) {
        throw std::runtime_error("Material 1 has modifiers");
    }
    if (!surface_contact_infos_.try_emplace(CommutativeMaterialPair{material0, material1}, std::move(info)).second) {
        throw std::runtime_error("Surface contact info already stored for materials");
    }
}

const SurfaceContactInfo* SurfaceContactDb::get_contact_info(
    PhysicsMaterial material0,
    PhysicsMaterial material1) const
{
    auto sit = surface_contact_infos_.find(CommutativeMaterialPair{
        material0 & PhysicsMaterial::SURFACE_MASK,
        material1 & PhysicsMaterial::SURFACE_MASK});
    if (sit == surface_contact_infos_.end()) {
        return nullptr;
    }
    return &sit->second;
}
