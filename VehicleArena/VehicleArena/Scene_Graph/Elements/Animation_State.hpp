// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Scene_Graph/Animation/Animation_Frame.hpp>
#include <VehicleArena/Scene_Graph/Animation/Aperiodic_Reference_Time.hpp>
#include <VehicleArena/Scene_Graph/Animation/Periodic_Reference_Time.hpp>
#include <cmath>
#include <variant>

namespace VA {

struct AnimationState {
    const VariableAndHash<std::string> periodic_skelletal_animation_name;
    const VariableAndHash<std::string> aperiodic_skelletal_animation_name;
    PeriodicAnimationFrame periodic_skelletal_animation_frame {
        AnimationFrame{
            .begin = NAN,
            .end = NAN,
            .time = NAN}};
    AperiodicAnimationFrame aperiodic_animation_frame {
        AnimationFrame{
            .begin = NAN,
            .end = NAN,
            .time = NAN}};
    std::variant<std::monostate, PeriodicReferenceTime, AperiodicReferenceTime>
        reference_time;
    const bool delete_node_when_aperiodic_animation_finished = false;
};

enum class AnimationStateAlreadyExistsBehavior {
    REPLACE,
    THROW
};

}
