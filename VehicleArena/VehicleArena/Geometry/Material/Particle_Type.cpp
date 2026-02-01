// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Particle_Type.hpp"
#include <map>
#include <stdexcept>

using namespace VA;

ParticleType VA::particle_type_from_string(const std::string& s) {
    static std::map<std::string, ParticleType> m{
        {"none", ParticleType::NONE},
        {"smoke", ParticleType::SMOKE},
        {"skidmark", ParticleType::SKIDMARK},
        {"water_wave", ParticleType::WATER_WAVE},
        {"sea_spray", ParticleType::SEA_SPRAY},
    };
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown particle type: \"" + s + '"');
    }
    return it->second;
}

std::string VA::particle_type_to_string(ParticleType s) {
    switch (s) {
    case ParticleType::NONE:
        return "none";
    case ParticleType::SMOKE:
        return "smoke";
    case ParticleType::SKIDMARK:
        return "skidmark";
    case ParticleType::WATER_WAVE:
        return "water_wave";
    case ParticleType::SEA_SPRAY:
        return "sea_spray";
    }
    throw std::runtime_error("Unknown particle type: " + std::to_string((int)s));
}
