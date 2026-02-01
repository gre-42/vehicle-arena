// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Aim_At.hpp"
#include <VehicleArena/Physics/Advance_Times/Movables/Aim_At.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>
#include <stdexcept>

using namespace VA;

bool VA::has_aim_at(const DanglingBaseClassRef<SceneNode>& node) {
    return node->has_sticky_absolute_observer();
}

AimAt& VA::get_aim_at(const DanglingBaseClassRef<SceneNode>& node) {
    auto aim_at = dynamic_cast<AimAt*>(&node->get_sticky_absolute_observer());
    if (aim_at == nullptr) {
        throw std::runtime_error("Sticky absolute observer is not an aim_at");
    }
    return *aim_at;
}
