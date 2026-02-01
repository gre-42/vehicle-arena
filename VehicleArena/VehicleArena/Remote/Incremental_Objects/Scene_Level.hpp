// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <functional>
#include <string>

namespace VA {

struct LocalSceneLevel {
    LocalSceneLevel();
    LocalSceneLevel(
        std::string name,
        uint8_t reload_counter = 0);
    ~LocalSceneLevel();
    bool reload_required(const LocalSceneLevel& other_level) const;
    std::string name;
    uint8_t reload_count;
};

enum class LocalSceneLevelLoadStatus: uint8_t {
    LOADING = 0xCA,
    RUNNING = 0x2B
};

class SceneLevelSelector: public virtual DanglingBaseClass {
public:
    SceneLevelSelector(
        LocalSceneLevel local_scene_level,
        std::function<void()> on_schedule_load_scene);
    ~SceneLevelSelector();
    LocalSceneLevel get_local_scene_level() const;
    LocalSceneLevel get_next_scene_level() const;
    std::string get_next_scene_name() const;
    void server_set_next_scene_level(std::string level);
    void client_schedule_load_scene_level(LocalSceneLevel level);
    bool reload_required(const LocalSceneLevel& other_level) const;
    LocalSceneLevelLoadStatus load_status() const;
    void notify_level_loaded();
private:
    mutable SafeAtomicRecursiveSharedMutex mutex_;
    LocalSceneLevelLoadStatus load_status_;
    LocalSceneLevel local_scene_level_;
    LocalSceneLevel next_scene_level_;
    std::function<void()> on_schedule_load_scene_;
};

}
