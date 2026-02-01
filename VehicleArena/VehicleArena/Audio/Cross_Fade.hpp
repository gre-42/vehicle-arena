// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Audio/Audio_Source.hpp>
#include <VehicleArena/Math/Fixed_Point_Number.hpp>
#include <VehicleArena/Memory/Event_Emitter.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <VehicleArena/Threads/J_Thread.hpp>
#include <cmath>
#include <functional>
#include <iosfwd>
#include <list>
#include <memory>
#include <optional>

namespace VA {

class AudioBuffer;
template <class TPosition>
struct AudioSourceState;

using Gain = FixedPointNumber<int32_t, (1 << 13)>;

struct AudioSourceAndGain {
    const AudioBuffer* audio_buffer;
    Gain gain;
    float gain_factor;
    float buffer_frequency;
    std::unique_ptr<AudioSource> source;
    void apply_gain();
};

class CrossFade {
    CrossFade(const CrossFade &) = delete;
    CrossFade &operator=(const CrossFade &) = delete;
public:
    explicit CrossFade(
        PositionRequirement position_requirement,
        std::function<bool()> paused,
        EventEmitter<>& paused_changed,
        float dgain = 0.02f);
    ~CrossFade();
    void start_background_thread(float dt = 0.01f);
    void advance_time(float dt);
    void play(
        const AudioBuffer &audio_buffer,
        float gain_factor = 1.f,
        float pitch = 1.f,
        float buffer_frequency = NAN,
        float alpha = NAN);
    void stop();
    void set_position(const AudioSourceState<ScenePos> &position);
    void print(std::ostream& ostr) const;

private:
    void update_gain_unsafe(float dgain);
    void update_pitch_unsafe(float pitch);
    void print_unsafe(std::ostream& ostr) const;

    PositionRequirement position_requirement_;
    Gain total_gain_;
    float dgain_;
    std::list<AudioSourceAndGain> sources_;
    mutable FastMutex mutex_;
    std::function<bool()> paused_;
    std::optional<JThread> fader_;
    EventReceiverDeletionToken<> erdt_;
};

}
