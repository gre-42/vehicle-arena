// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Audio/Audio_Entity_State.hpp>
#include <VehicleArena/Audio/Audio_Source.hpp>
#include <VehicleArena/Math/Fixed_Point_Number.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Event_Emitter.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <functional>
#include <list>
#include <memory>

namespace VA {

template <class... Args>
class EventReceiverDeletionToken;
class AudioBuffer;
class AudioLowpass;
template <class T>
struct Interval;
enum class AudioPeriodicity;

struct AudioSourceAndPosition {
    AudioSourceAndPosition(
        const AudioBuffer& buffer,
        PositionRequirement position_requirement,
        float alpha,
        const AudioSourceState<ScenePos>& position);
    ~AudioSourceAndPosition();
    AudioSource source;
    AudioSourceState<ScenePos> position;
};

class OneShotAudio final: public IAdvanceTime, public DanglingBaseClass {
    OneShotAudio(const OneShotAudio&) = delete;
    OneShotAudio &operator=(const OneShotAudio&) = delete;
public:
    explicit OneShotAudio(
        PositionRequirement position_requirement,
        std::function<bool()> paused,
        EventEmitter<>& paused_changed);
    ~OneShotAudio();
    void advance_time();
    virtual void advance_time(float dt, const StaticWorld& world) override;
    std::shared_ptr<AudioSourceAndPosition> play(
        const AudioBuffer& audio_buffer,
        const AudioLowpass* lowpass,
        const AudioSourceState<ScenePos>& position,
        AudioPeriodicity periodicity,
        const std::optional<Interval<float>>& distance_clamping,
        float gain,
        float alpha = NAN);
    void stop();
private:
    PositionRequirement position_requirement_;
    std::list<std::shared_ptr<AudioSourceAndPosition>> sources_;
    mutable FastMutex mutex_;
    std::function<bool()> paused_;
    EventReceiverDeletionToken<> erdt_;
};

}
