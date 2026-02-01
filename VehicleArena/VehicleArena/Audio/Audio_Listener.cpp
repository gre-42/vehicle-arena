// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Listener.hpp"
#include <VehicleArena/Audio/CHK.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <mutex>

using namespace VA;

FastMutex AudioListener::mutex_;
bool AudioListener::muted_ = false;
float AudioListener::gain_ = 1.f;
std::optional<AudioListenerState> AudioListener::listener_inverse_state_ = std::nullopt;

void AudioListener::set_transformation(const AudioListenerState& listener_state) {
    std::scoped_lock lock{mutex_};
    listener_inverse_state_ = AudioListenerState{
        .pose = listener_state.pose.inverted(),
        .velocity = -listener_state.pose.irotate(listener_state.velocity)
    };
    // AL_CHK(alListenerfv(AL_POSITION, trafo.t().flat_begin()));
    // float orientation[6] = {
    //     -trafo.R(0, 2),
    //     -trafo.R(1, 2),
    //     -trafo.R(2, 2),
    //     trafo.R(0, 1),
    //     trafo.R(1, 1),
    //     trafo.R(2, 1)
    // };
    // AL_CHK(alListenerfv(AL_ORIENTATION, orientation));
}

std::optional<AudioSourceState<float>> AudioListener::get_relative_position(const AudioSourceState<ScenePos>& state) {
    std::scoped_lock lock{mutex_};
    if (!listener_inverse_state_.has_value()) {
        return std::nullopt;
    }
    const auto& il = *listener_inverse_state_;
    return AudioSourceState<float>{
        .position = il.pose.transform(state.position).casted<float>(),
        .velocity = il.velocity + il.pose.rotate(state.velocity)
    };
}

void AudioListener::set_gain(float f) {
    std::scoped_lock lock{mutex_};
    AL_CHK(alListenerf(AL_GAIN, f));
    gain_ = f;
}

void AudioListener::mute() {
    std::scoped_lock lock{mutex_};
    if (!muted_) {
        AL_CHK(alListenerf(AL_GAIN, 0.f));
        muted_ = true;
    }
}

void AudioListener::unmute() {
    std::scoped_lock lock{mutex_};
    if (muted_) {
        AL_CHK(alListenerf(AL_GAIN, gain_));
        muted_ = false;
    }
}
