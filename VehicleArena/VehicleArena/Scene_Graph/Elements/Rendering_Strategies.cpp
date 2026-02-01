// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Rendering_Strategies.hpp"
#include <map>
#include <stdexcept>

using namespace VA;

static std::string rendering_strategy_to_string(RenderingStrategies rendering_strategy) {
    switch (rendering_strategy) {
    case RenderingStrategies::NONE:
        return "none";
    case RenderingStrategies::MESH_ONCE:
        return "mesh_once";
    case RenderingStrategies::MESH_SORTED_CONTINUOUSLY:
        return "mesh_sorted_continuously";
    case RenderingStrategies::INSTANCES_ONCE:
        return "instances_once";
    case RenderingStrategies::INSTANCES_SORTED_CONTINUOUSLY:
        return "instances_sorted_continuously";
    case RenderingStrategies::OBJECT:
        return "object";
    }
    throw std::runtime_error("Unknown rendering strategy: " + std::to_string((int)rendering_strategy));
}

std::string VA::rendering_strategies_to_string(RenderingStrategies rendering_strategies) {
    std::string result = "";
    using UT = std::underlying_type<RenderingStrategies>::type;
    for (UT ucandidate = 1; ucandidate != 0; ucandidate <<= 1) {
        auto candidate = (RenderingStrategies)ucandidate;
        if (any(rendering_strategies & candidate)) {
            if (result.empty()) {
                result = rendering_strategy_to_string(candidate);
            } else {
                result += '|' + rendering_strategy_to_string(candidate);
            }
        }
    }
    if (result.empty()) {
        return rendering_strategy_to_string(RenderingStrategies::NONE);
    }
    return result;
}

RenderingStrategies VA::rendering_strategy_from_string(const std::string& s) {
    static const std::map<std::string, RenderingStrategies> m{
        {"none", RenderingStrategies::NONE},
        {"mesh_once", RenderingStrategies::MESH_ONCE},
        {"mesh_sorted_continuously", RenderingStrategies::MESH_SORTED_CONTINUOUSLY},
        {"instances_once", RenderingStrategies::INSTANCES_ONCE},
        {"instances_sorted_continuously", RenderingStrategies::INSTANCES_SORTED_CONTINUOUSLY},
        {"object", RenderingStrategies::OBJECT}};
    auto it = m.find(s);
    if (it == m.end()) {
        throw std::runtime_error("Unknown rendering strategy: \"" + s + '"');
    }
    return it->second;
}
