// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <algorithm>
#include <compare>
#include <cstddef>
#include <cstdint>
#include <map>

namespace VA {

class SmokeParticleGenerator;
class RigidBodyVehicle;
enum class PhysicsMaterial: uint32_t;
struct SurfaceContactInfo;

class CommutativeMaterialPair {
public:
    CommutativeMaterialPair(
        PhysicsMaterial a,
        PhysicsMaterial b)
        : material0_{ std::min(a, b) }
        , material1_{ std::max(a, b) }
    {}
    std::strong_ordering operator<=>(const CommutativeMaterialPair &) const = default;

private:
    PhysicsMaterial material0_; 
    PhysicsMaterial material1_;
};

class SurfaceContactDb {
public:
    SurfaceContactDb();
    ~SurfaceContactDb();
    void store_contact_info(
        SurfaceContactInfo info,
        PhysicsMaterial material0,
        PhysicsMaterial material1);
    const SurfaceContactInfo* get_contact_info(
        PhysicsMaterial material0,
        PhysicsMaterial material1) const;
private:
    std::map<CommutativeMaterialPair, SurfaceContactInfo> surface_contact_infos_;
};

}
