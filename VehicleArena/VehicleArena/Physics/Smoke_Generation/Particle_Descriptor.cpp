// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Particle_Descriptor.hpp"
#include <VehicleArena/Geometry/Material/Particle_Type.hpp>
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <map>

namespace ParticleDescriptorArgs {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(resource);
DECLARE_ARGUMENT(air_resistance_halflife);
DECLARE_ARGUMENT(animation_duration);
DECLARE_ARGUMENT(rotation);
DECLARE_ARGUMENT(type);
}

using namespace VA;

ParticleRotation VA::particle_rotation_from_string(const std::string& s) {
    static const std::map<std::string, ParticleRotation> m{
        {"emitter", ParticleRotation::EMITTER},
        {"random_yangle", ParticleRotation::RANDOM_YANGLE},
    };
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown particle rotation: \"" + s + '"');
    }
    return it->second;
}

void VA::from_json(const nlohmann::json& j, ParticleDescriptor& item) {
    JsonView jv{ j };
    jv.validate(ParticleDescriptorArgs::options);

    item.resource_name = jv.at<std::string>(ParticleDescriptorArgs::resource);
    item.air_resistance_halflife = jv.at<float>(ParticleDescriptorArgs::air_resistance_halflife) * seconds;
    item.animation_duration = jv.at<float>(ParticleDescriptorArgs::animation_duration, NAN) * seconds;
    item.rotation = particle_rotation_from_string(jv.at<std::string>(ParticleDescriptorArgs::rotation, "emitter"));
    item.type = particle_type_from_string(jv.at<std::string>(ParticleDescriptorArgs::type));
}
