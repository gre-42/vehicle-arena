// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Resources.hpp"
#include <VehicleArena/Audio/Audio_Buffer.hpp>
#include <VehicleArena/Audio/Audio_Buffer_Sequence_With_Hysteresis.hpp>
#include <VehicleArena/Audio/Audio_Equalizer.hpp>
#include <VehicleArena/Audio/Audio_File_Sequence.hpp>
#include <VehicleArena/Audio/Audio_Lowpass.hpp>
#include <list>
#include <mutex>
#include <stdexcept>

using namespace VA;

AudioResources::AudioResources()
    : buffer_filenames_{"Buffer filename"}
    , buffer_meta_{"Buffer meta information"}
    , buffers_{"Buffer"}
    , buffer_sequence_filenames_{"Buffer sequence filename"}
    , buffer_sequences_{"Buffer sequence"}
    , equalizer_parameters_{"Equalizer parameters"}
    , equalizers_{"Equalizers"}
    , lowpass_parameters_{"Lowpass parameters"}
    , lowpasses_{"Lowpass"}
{}

AudioResources::~AudioResources() = default;

void AudioResources::add_buffer(
    const VariableAndHash<std::string>& name,
    const std::string& filename,
    float gain,
    const std::optional<Interval<float>>& distance_clamping,
    const std::optional<VariableAndHash<std::string>>& lowpass)
{
    std::unique_lock lock{ mutex_ };
    buffer_filenames_.add(
        name,
        filename,
        gain,
        distance_clamping,
        lowpass);
}

const AudioMetaInformation& AudioResources::get_buffer_meta(const VariableAndHash<std::string>& name) const {
    {
        std::shared_lock lock{ mutex_ };
        if (auto it = buffer_meta_.try_get(name); it != nullptr) {
            return *it;
        }
    }
    std::unique_lock lock{ mutex_ };
    if (auto it = buffer_meta_.try_get(name); it != nullptr) {
        return *it;
    }
    const auto& file = buffer_filenames_.get(name);
    std::shared_ptr<AudioLowpass> lowpass;
    if (file.lowpass.has_value()) {
        lowpass = get_lowpass(*file.lowpass);
    }
    return buffer_meta_.add(name, file.gain, file.distance_clamping, lowpass);
}

std::shared_ptr<AudioBuffer> AudioResources::get_buffer(const VariableAndHash<std::string>& name) const {
    {
        std::shared_lock lock{ mutex_ };
        if (auto it = buffers_.try_get(name); it != nullptr) {
            return *it;
        }
    }
    std::unique_lock lock{ mutex_ };
    if (auto it = buffers_.try_get(name); it != nullptr) {
        return *it;
    }
    auto& fit = buffer_filenames_.get(name);
    return buffers_.add(name, AudioBuffer::from_wave(fit.filename));
}

void AudioResources::preload_buffer(const VariableAndHash<std::string>& name) const {
    get_buffer(name);
}

void AudioResources::add_buffer_sequence(
    const VariableAndHash<std::string>& name,
    const std::string& filename,
    float gain,
    const std::optional<Interval<float>>& distance_clamping,
    float hysteresis_step)
{
    if (gain < 0.f) {
        throw std::runtime_error("Attempt to set negative buffer sequence gain");
    }
    if (gain > 1.f) {
        throw std::runtime_error("Attempt to set buffer sequence gain greater than 1");
    }
    std::unique_lock lock{mutex_};
    buffer_sequence_filenames_.add(name, filename, gain, distance_clamping, hysteresis_step);
}

float AudioResources::get_buffer_sequence_gain(const VariableAndHash<std::string>& name) const {
    std::shared_lock lock{mutex_};
    return buffer_sequence_filenames_.get(name).gain;
}

std::shared_ptr<AudioBufferSequenceWithHysteresis> AudioResources::get_buffer_sequence(const VariableAndHash<std::string>& name) const {
    {
        std::shared_lock lock{mutex_};
        if (auto it = buffer_sequences_.try_get(name); it != nullptr) {
            return *it;
        }
    }
    std::unique_lock lock{mutex_};
    if (auto it = buffer_sequences_.try_get(name); it != nullptr) {
        return *it;
    }
    auto& it = buffer_sequence_filenames_.get(name);
    auto items = load_audio_file_sequence(it.filename);
    std::list<AudioBufferAndFrequency> buffers;
    for (const auto& i : items) {
        buffers.push_back(AudioBufferAndFrequency{
            .buffer = AudioBuffer::from_wave(i.filename),
            .frequency = i.frequency});
    }
    auto seq = std::make_shared<AudioBufferSequenceWithHysteresis>(std::vector(buffers.begin(), buffers.end()), it.hysteresis_step);
    return buffer_sequences_.add(name, seq);
}

void AudioResources::add_equalizer(
    const VariableAndHash<std::string>& name,
    const AudioEqualizerInformation& equalizer) const
{
    std::shared_lock lock{mutex_};
    equalizer_parameters_.add(name, equalizer);
}

std::shared_ptr<AudioEqualizer>
    AudioResources::get_equalizer(const VariableAndHash<std::string>& name) const
{
    {
        std::shared_lock lock{mutex_};
        if (auto it = equalizers_.try_get(name); it != nullptr) {
            return *it;
        }
    }
    std::unique_lock lock{mutex_};
    if (auto it = equalizers_.try_get(name); it != nullptr) {
        return *it;
    }
    auto equalizer = AudioEqualizer::create(equalizer_parameters_.get(name));
    return equalizers_.add(name, equalizer);
}

void AudioResources::add_lowpass(
    const VariableAndHash<std::string>& name,
    const AudioLowpassInformation& lowpass) const
{
    std::shared_lock lock{mutex_};
    lowpass_parameters_.add(name, lowpass);
}

std::shared_ptr<AudioLowpass>
    AudioResources::get_lowpass(const VariableAndHash<std::string>& name) const
{
    {
        std::shared_lock lock{mutex_};
        if (auto it = lowpasses_.try_get(name); it != nullptr) {
            return *it;
        }
    }
    std::unique_lock lock{mutex_};
    if (auto it = lowpasses_.try_get(name); it != nullptr) {
        return *it;
    }
    auto lowpass = AudioLowpass::create(lowpass_parameters_.get(name));
    return lowpasses_.add(name, lowpass);
}
