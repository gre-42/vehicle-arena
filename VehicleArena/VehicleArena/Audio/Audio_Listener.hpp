// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Audio/Audio_Entity_State.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <optional>

namespace VA {

class AudioListener {
    AudioListener() = delete;
    AudioListener(const AudioListener &) = delete;
    AudioListener &operator=(const AudioListener &) = delete;

public:
    static void set_transformation(const AudioListenerState& state);
    static std::optional<AudioSourceState<float>> get_relative_position(const AudioSourceState<ScenePos> &state);
    static void set_gain(float f);
    static void mute();
    static void unmute();

private:
    static FastMutex mutex_;
    static bool muted_;
    static float gain_;
    static std::optional<AudioListenerState> listener_inverse_state_;
};

}
