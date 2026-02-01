// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Buffer_Sequence.hpp"
#include <algorithm>
#include <stdexcept>

using namespace VA;

PitchAdjustmentStrategy VA::pitch_adjustment_strategy_from_string(const std::string& s) {
    if (s == "rounding") {
        return PitchAdjustmentStrategy::ROUNDING;
    }
    if (s == "up_sampling") {
        return PitchAdjustmentStrategy::UP_SAMPLING;
    }
    if (s == "down_sampling") {
        return PitchAdjustmentStrategy::DOWN_SAMPLING;
    }
    throw std::runtime_error("Unknown pitch adjustment strategy: \"" + s + '"');
}

AudioBufferSequence::AudioBufferSequence(std::vector<AudioBufferAndFrequency> buffers)
: buffers_{std::move(buffers)}
{}

const AudioBufferAndFrequency& AudioBufferSequence::get_buffer_and_frequency(
    float frequency,
    PitchAdjustmentStrategy strategy) const
{
    if (buffers_.empty()) {
        throw std::runtime_error("Audio buffer vector is empty");
    }
    auto it = std::lower_bound(
        buffers_.begin(),
        buffers_.end(),
        frequency,
        [](const AudioBufferAndFrequency& l, float f){return l.frequency < f;});
    if (it == buffers_.end()) {
        return buffers_.back();
    }
    if (it == buffers_.begin()) {
        return *it;
    }
    if (strategy == PitchAdjustmentStrategy::ROUNDING) {
        float d_left = frequency - (it - 1)->frequency;
        float d_right = it->frequency - frequency;
        if (d_left < d_right) {
            return *(it - 1);
        } else {
            return *it;
        }
    }
    if (strategy == PitchAdjustmentStrategy::UP_SAMPLING) {
        return *it;
    }
    if (strategy == PitchAdjustmentStrategy::DOWN_SAMPLING) {
        return *(it - 1);
    }
    throw std::runtime_error("Unknown pitch adjustment strategy");
}
