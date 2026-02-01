// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Audio/Lazy_One_Shot_Audio.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Math/Interp.hpp>
#include <VehicleArena/Physics/Smoke_Generation/Particle_Descriptor.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace VA {

struct SurfaceSmokeRule {
    // Has a standard constructor for JSON parsing (needs to be default-constructible).
    Interp<float> smoke_particle_frequency = Interp<float>{ {}, {} };
    Interp<float> smoke_particle_layer = Interp<float>{ {}, {} };
    Interp<float> smoke_particle_velocity = Interp<float>{ {}, {} };
};

struct SurfaceSmokeVisual {
    ParticleDescriptor particle;
    std::string smoke_particle_instance_prefix;
};

enum class SurfaceSmokeAffinity {
    PAIR,
    TIRE
};

SurfaceSmokeAffinity surface_smoke_affinity_from_string(const std::string& s);

struct SurfaceSmokeInfo {
    std::optional<SurfaceSmokeVisual> visual;
    std::unique_ptr<LazyOneShotAudio> audio;
    std::optional<VariableAndHash<std::string>> audio_resource_name;
    SurfaceSmokeAffinity affinity;
    SurfaceSmokeRule vehicle_velocity;
    SurfaceSmokeRule tire_velocity;
};

struct SurfaceContactInfo {
    float stiction_factor;      // for tires (tires store their stiction coefficient themselves)
    float stiction_coefficient; // for everything except tires
    float friction_coefficient; // for everything except tires
    std::vector<SurfaceSmokeInfo> emission;
};

}
