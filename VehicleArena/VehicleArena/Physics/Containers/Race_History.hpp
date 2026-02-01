// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Initialization/Default_Uninitialized_Vector.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Containers/Race_Identifier.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <iosfwd>
#include <list>
#include <optional>
#include <string>

namespace VA {

struct TrackElement;
class SceneNodeResources;
enum class RaceState;
class Translator;

struct LapTimeEvent {
    float race_time_seconds;
    std::string player_name;
    std::string vehicle;
    UUVector<FixedArray<float, 3>> vehicle_colors;
};

struct LapTimeEventAndId {
    LapTimeEvent event;
    size_t id;
    std::list<float> lap_times_seconds;
    bool playback_exists;
};

struct LapTimeEventAndIdAndMfilename {
    LapTimeEvent event;
    std::string m_filename;
};

struct RaceIdentifier;
struct RaceConfiguration;
enum class ScoreBoardConfiguration;

class RaceHistory {
public:
    explicit RaceHistory(
        size_t max_tracks,
        bool save_playback,
        const SceneNodeResources& scene_node_resources,
        const RaceIdentifier& race_identifier,
        std::shared_ptr<Translator> translator);
    ~RaceHistory();
    RaceState notify_lap_finished(
        const LapTimeEvent& lap_time_event,
        const std::list<float>& lap_times_seconds,
        const std::list<TrackElement>& track);
    uint32_t rank(float race_time_seconds) const;
    std::string get_level_history(ScoreBoardConfiguration score_board_config) const;
    std::optional<LapTimeEventAndIdAndMfilename> get_winner_track_filename(size_t position) const;
    void set_race_identifier_and_reload(const RaceIdentifier& race_identifier);
    void start_race(const RaceConfiguration& race_configuration);
    const RaceIdentifier& race_identifier() const;
private:
    std::string race_dirname() const;
    std::string stats_json_filename() const;
    std::string config_json_filename() const;
    std::string track_m_filename(size_t id) const;
    void save_and_discard();
    size_t max_tracks_;
    bool save_playback_;
    std::list<LapTimeEventAndId> lap_time_events_;
    const SceneNodeResources& scene_node_resources_;
    RaceIdentifier race_identifier_;
    std::shared_ptr<Translator> translator_;
    mutable SafeAtomicRecursiveSharedMutex mutex_;
};

}
