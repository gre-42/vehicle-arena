// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Aggregate_Mode.hpp"
#include <map>
#include <stdexcept>

using namespace VA;

AggregateMode VA::aggregate_mode_from_string(const std::string& str) {
    static const std::map<std::string, AggregateMode> m{
        {"none", AggregateMode::NONE},
        {"once", AggregateMode::ONCE},
        {"sorted", AggregateMode::SORTED_CONTINUOUSLY},
        {"instances_once", AggregateMode::INSTANCES_ONCE},
        {"instances_sorted", AggregateMode::INSTANCES_SORTED_CONTINUOUSLY},
        {"node_object", AggregateMode::NODE_OBJECT},
        {"node_triangles", AggregateMode::NODE_TRIANGLES},
        {"object_mask", AggregateMode::OBJECT_MASK},
        {"instances_mask", AggregateMode::INSTANCES_MASK}};
    auto it = m.find(str);
    if (it == m.end()) {
        throw std::runtime_error("Unknown aggregate mode: \"" + str + '"');
    }
    return it->second;
}

std::string VA::aggregate_mode_to_string(AggregateMode aggregate_mode) {
    switch (aggregate_mode) {
    case AggregateMode::NONE: return "none";
    case AggregateMode::ONCE: return "once";
    case AggregateMode::SORTED_CONTINUOUSLY: return "sorted";
    case AggregateMode::INSTANCES_ONCE: return "instances_once";
    case AggregateMode::INSTANCES_SORTED_CONTINUOUSLY: return "instances_sorted";
    case AggregateMode::NODE_OBJECT: return "node_object";
    case AggregateMode::NODE_TRIANGLES: return "node_triangles";
    case AggregateMode::OBJECT_MASK: return "object_mask";
    case AggregateMode::INSTANCES_MASK: return "instances_mask";
    }
    throw std::runtime_error("Unknown aggregate mode: \"" + std::to_string((int)aggregate_mode) + '"');
}
