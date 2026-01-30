// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Misc.hpp"
#include <VehicleArena/Strings/String.hpp>

using namespace VA;

std::string VA::get_multiline_string(const nlohmann::json& j) {
    return VA::join("\n", j.get<std::vector<std::string>>());
}

void VA::validate(const nlohmann::json& j, const std::set<std::string_view>& known_keys, std::string_view prefix) {
    if (j.type() != nlohmann::detail::value_t::object) {
        throw std::runtime_error(std::string{ prefix } + "JSON is not of type object");
    }
    for (const auto& [key, _] : j.items()) {
        if (!key.starts_with("#") && !known_keys.contains(key)) {
            throw std::runtime_error(std::string{ prefix } + "Unknown key in JSON: \"" + key + '"');
        }
    }
}

void VA::validate_complement(const nlohmann::json& j, const std::set<std::string_view>& known_keys, std::string_view prefix) {
    if (j.type() != nlohmann::detail::value_t::object) {
        throw std::runtime_error(std::string{ prefix } + "JSON is not of type object");
    }
    for (const auto& key : known_keys) {
        if (j.contains(key)) {
            throw std::runtime_error(std::string{ prefix } + "Key already exists in JSON: \"" + std::string{ key } + '"');
        }
    }
}
