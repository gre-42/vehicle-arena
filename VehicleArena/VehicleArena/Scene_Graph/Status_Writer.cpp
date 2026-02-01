// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Status_Writer.hpp"
#include <VehicleArena/Strings/String.hpp>
#include <map>
#include <stdexcept>

using namespace VA;

void StatusWriter::write_status(std::ostream& ostr, StatusComponents status_components, const StaticWorld& world) const {
    throw std::runtime_error("StatusWriter::write_status not implemented");
}

float StatusWriter::get_value(StatusComponents status_components) const {
    throw std::runtime_error("StatusWriter::get_value not implemented");
}

StatusComponents single_status_component_from_string(const std::string& s) {
    static const std::map<std::string, StatusComponents> map{
        {"time", StatusComponents::TIME},
        {"position", StatusComponents::POSITION},
        {"speed", StatusComponents::SPEED},
        {"health", StatusComponents::HEALTH},
        {"diameter", StatusComponents::DIAMETER},
        {"energy", StatusComponents::ENERGY},
        {"driver_name", StatusComponents::DRIVER_NAME},
        {"angular_velocity", StatusComponents::ANGULAR_VELOCITY},
        {"wheel_angular_velocity", StatusComponents::WHEEL_ANGULAR_VELOCITY},
        {"abs_angular_velocity", StatusComponents::ABS_ANGULAR_VELOCITY}
    };
    auto it = map.find(s);
    if (it == map.end()) {
        throw std::runtime_error("Unknown status component: \"" + s + '"');
    }
    return it->second;
}

StatusComponents VA::status_components_from_string(const std::string& s) {
    static const DECLARE_REGEX(re, "\\|");
    StatusComponents result = StatusComponents::NONE;
    for (const auto& m : string_to_list(s, re)) {
        result |= single_status_component_from_string(m);
    }
    return result;
}
