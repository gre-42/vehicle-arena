// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Buffer_Sequence_With_Hysteresis.hpp"
#include <stdexcept>

using namespace VA;

AudioBufferSequenceWithHysteresis::AudioBufferSequenceWithHysteresis(
    std::vector<AudioBufferAndFrequency> buffers,
    float hysteresis_step)
    : previous_result_{ nullptr }
    , hysteresis_step_{ hysteresis_step }
    , seq_{ std::move(buffers) }
{}

const AudioBufferAndFrequency& AudioBufferSequenceWithHysteresis::get_buffer_and_frequency(
    float frequency,
    PitchAdjustmentStrategy strategy)
{
    if (frequency < 0.f) {
        throw std::runtime_error("Requested negative audio frequency");
    }
    if ((previous_result_ == nullptr) ||
        (std::abs(previous_result_->frequency - frequency) / (previous_result_->frequency + 1e-6) > hysteresis_step_))
    {
        previous_result_ = &seq_.get_buffer_and_frequency(frequency, strategy);
    }
    return *previous_result_;
}
