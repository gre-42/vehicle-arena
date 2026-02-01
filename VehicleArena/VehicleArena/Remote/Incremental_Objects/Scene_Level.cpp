// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Scene_Level.hpp"
#include <mutex>
#include <stdexcept>

using namespace VA;

LocalSceneLevel::LocalSceneLevel()
    : LocalSceneLevel{"", 0}
{}

LocalSceneLevel::LocalSceneLevel(
    std::string name,
    uint8_t reload_count)
    : name{ std::move(name) }
    , reload_count{ reload_count }
{}

LocalSceneLevel::~LocalSceneLevel() = default;

bool LocalSceneLevel::reload_required(const LocalSceneLevel& other_level) const
{
    return
        (reload_count != other_level.reload_count) ||
        (name != other_level.name);
}

SceneLevelSelector::SceneLevelSelector(
    LocalSceneLevel local_scene_level,
    std::function<void()> on_schedule_load_scene)
    : load_status_{ LocalSceneLevelLoadStatus::LOADING }
    , local_scene_level_{ std::move(local_scene_level) }
    , on_schedule_load_scene_{ std::move(on_schedule_load_scene) }
{}

SceneLevelSelector::~SceneLevelSelector() = default;

LocalSceneLevel SceneLevelSelector::get_local_scene_level() const {
    std::shared_lock lock{ mutex_ };
    return local_scene_level_;
}

LocalSceneLevel SceneLevelSelector::get_next_scene_level() const {
    std::shared_lock lock{ mutex_ };
    return next_scene_level_;
}

std::string SceneLevelSelector::get_next_scene_name() const {
    std::shared_lock lock{ mutex_ };
    return next_scene_level_.name;
}

void SceneLevelSelector::server_set_next_scene_level(std::string level) {
    std::scoped_lock lock{ mutex_ };
    next_scene_level_.name = std::move(level);
    next_scene_level_.reload_count = local_scene_level_.reload_count + 1;
}

void SceneLevelSelector::client_schedule_load_scene_level(LocalSceneLevel level) {
    std::scoped_lock lock{ mutex_ };
    if (!on_schedule_load_scene_) {
        throw std::runtime_error("on_schedule_load_scene not set");
    }
    next_scene_level_ = std::move(level);
    if (on_schedule_load_scene_) {
        on_schedule_load_scene_();
    }
}

bool SceneLevelSelector::reload_required(const LocalSceneLevel& other_level) const {
    std::shared_lock lock{ mutex_ };
    return local_scene_level_.reload_required(other_level);
}

LocalSceneLevelLoadStatus SceneLevelSelector::load_status() const {
    std::shared_lock lock{ mutex_ };
    return load_status_;
}

void SceneLevelSelector::notify_level_loaded() {
    std::scoped_lock lock{ mutex_ };
    load_status_ = LocalSceneLevelLoadStatus::RUNNING;
}
