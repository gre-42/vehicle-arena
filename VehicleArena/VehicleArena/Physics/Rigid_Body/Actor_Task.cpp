// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Actor_Task.hpp"
#include <map>
#include <stdexcept>

using namespace VA;

ActorTask VA::actor_task_from_string(const std::string& str) {
    static const std::map<std::string, ActorTask> m{
        {"undefined", ActorTask::UNDEFINED},
        {"air_cruise", ActorTask::AIR_CRUISE},
        {"ground_cruise", ActorTask::GROUND_CRUISE},
        {"runway_accelerate", ActorTask::RUNWAY_ACCELERATE},
        {"runway_takeoff", ActorTask::RUNWAY_TAKEOFF}
    };
    auto it = m.find(str);
    if (it == m.end()) {
        throw std::runtime_error("Unknown actor task: \"" + str + '"');
    }
    return it->second;
}

std::string VA::actor_task_to_string(ActorTask actor_task) {
    switch (actor_task) {
    case ActorTask::UNDEFINED:
        return "undefined";
    case ActorTask::AIR_CRUISE:
        return "air_cruise";
    case ActorTask::GROUND_CRUISE:
        return "ground_cruise";
    case ActorTask::RUNWAY_ACCELERATE:
        return "runway_accelerate";
    case ActorTask::RUNWAY_TAKEOFF:
        return "runway_takeoff";
    case ActorTask::END:
        ; // Fall through
    }
    throw std::runtime_error("Unknown actor state: " + std::to_string((int)actor_task));
}
