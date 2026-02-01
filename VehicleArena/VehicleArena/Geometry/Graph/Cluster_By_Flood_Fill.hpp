// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <list>
#include <unordered_map>

namespace VA {

template <class TNodes, class TIsNeighbor>
std::list<std::list<typename TNodes::value_type*>> cluster_by_flood_fill(
    TNodes& nodes,
    const TIsNeighbor& is_neighbor)
{
    using value_type = TNodes::value_type;
    using Neighbors = std::unordered_map<value_type*, std::list<value_type*>>;
    Neighbors neighbors;
    for (auto n0 = nodes.begin(); n0 != nodes.end(); ++n0) {
        auto& neighbors0 = neighbors[&*n0];
        auto n1 = n0;
        ++n1;
        while (n1 != nodes.end()) {
            if (is_neighbor(*n0, *n1)) {
                auto& neighbors1 = neighbors[&*n1];
                // linfo() << *n0 << " - " << *n1;
                neighbors0.push_back(&*n1);
                neighbors1.push_back(&*n0);
            }
            ++n1;
        }
    }
    std::list<std::list<value_type*>> result;
    Neighbors active_nodes;
    while (!neighbors.empty()) {
        if (active_nodes.empty()) {
            active_nodes.insert(neighbors.extract(neighbors.begin()));
            // linfo() << "take first " << *active_nodes.begin()->first;
            result.emplace_back();
        }
        while (!active_nodes.empty()) {
            auto active = active_nodes.extract(active_nodes.begin());
            // linfo() << "add " << *active.key();
            result.back().push_back(active.key());
            for (auto& n : active.mapped()) {
                // linfo() << "visit " << *n;
                auto nit = neighbors.find(n);
                if (nit != neighbors.end()) {
                    // linfo() << "activate " << *nit->first;
                    active_nodes.insert(neighbors.extract(nit));
                }
            }
        }
    }
    return result;
}

}
