// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Audio/OpenAL_al.h>
#include <cstddef>
#include <cstdint>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
class AudioBuffer;
class AudioLowpass;
template <class TPosition>
struct AudioSourceState;
template <class T>
struct Interval;

enum class PositionRequirement {
    WAITING_FOR_POSITION,
    POSITION_NOT_REQUIRED
};

class AudioSource {
    AudioSource(const AudioSource &) = delete;
    AudioSource &operator=(const AudioSource &) = delete;
    explicit AudioSource(
        PositionRequirement position_requirement,
        float alpha);
public:
    AudioSource(
        const AudioBuffer& buffer,
        PositionRequirement position_requirement,
        float alpha = 1.f);
    ~AudioSource();
    void set_loop(bool value);
    void set_gain(float f);
    void set_pitch(float f);
    void set_position(const AudioSourceState<float>& position);
    void set_distance_clamping(const Interval<float>& interval);
    void set_lowpass(const AudioLowpass& lowpass);
    void play();
    void pause();
    void unpause();
    void stop();
    void join();
    void mute();
    void unmute();
    bool stopped() const;
private:
    uint32_t nchannels_;
    ALuint source_;
    PositionRequirement position_requirement_;
    bool muted_;
    float gain_;
};

}
