// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Engine_Power_Intent.hpp"
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>
#include <map>
#include <ostream>

using namespace VA;

namespace KnownPowerIntentArgs {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(state);
DECLARE_ARGUMENT(surface_power);
DECLARE_ARGUMENT(drive_relaxation);
DECLARE_ARGUMENT(parking_brake_pulled);
}

float EnginePowerIntent::real_power(
    float engine_angular_velocity,
    float tires_angular_velocity,
    float max_surface_power) const
{
    return
        std::isnan(surface_power) ||
        std::isnan(tires_angular_velocity) ||
        std::isnan(max_surface_power) ||
        (sign(surface_power) == sign(tires_angular_velocity))
            ? 0.f
            : std::clamp(
                std::abs(surface_power),
                0.f,
                max_surface_power) * drive_relaxation;
}

std::ostream& VA::operator << (std::ostream& ostr, const EnginePowerIntent& engine_power_intent) {
    ostr << "state: " << (int)engine_power_intent.state << '\n';
    ostr << "surface_power: " << engine_power_intent.surface_power << '\n';
    ostr << "drive_relaxation: " << engine_power_intent.drive_relaxation << '\n';
    ostr << "parking_brake_pulled: " << engine_power_intent.parking_brake_pulled;
    return ostr;
}

void VA::from_json(const nlohmann::json& j, EngineState& s) {
    static const std::map<std::string, EngineState> m{
        {"on", EngineState::ON},
        {"off", EngineState::OFF}};
    auto str = j.get<std::string>();
    auto it = m.find(str);
    if (it == m.end()) {
        throw std::runtime_error("Unknown engine state: \"" + str + '"');
    }
    s = it->second;
}

void VA::from_json(const nlohmann::json& j, EnginePowerIntent& i) {
    JsonView jv{ j };
    jv.validate(KnownPowerIntentArgs::options);
    i.state = jv.at<EngineState>(KnownPowerIntentArgs::state);
    i.surface_power = jv.at<float>(KnownPowerIntentArgs::surface_power);
    i.drive_relaxation = jv.at<float>(KnownPowerIntentArgs::drive_relaxation);
    i.parking_brake_pulled = jv.at<float>(KnownPowerIntentArgs::parking_brake_pulled);
}
