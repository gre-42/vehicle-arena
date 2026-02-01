// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Weapon_Cycle.hpp"
#include <VehicleArena/Physics/Misc/Weapon_Cycle.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <stdexcept>

using namespace VA;

const WeaponCycle& VA::get_weapon_cycle(const DanglingBaseClassRef<const SceneNode>& node) {
    auto wc = dynamic_cast<WeaponCycle*>(&node->get_node_modifier());
    if (wc == nullptr) {
        throw std::runtime_error("Node modifier is not a list of weapon cycles");
    }
    return *wc;
}

WeaponCycle& VA::get_weapon_cycle(const DanglingBaseClassRef<SceneNode>& node) {
    return const_cast<WeaponCycle&>(get_weapon_cycle((const DanglingBaseClassRef<const SceneNode>&)node));
}

bool VA::has_weapon_cycle(const DanglingBaseClassRef<const SceneNode>& node) {
    return node->has_node_modifier();
}
