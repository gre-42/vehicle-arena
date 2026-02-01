// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Interval.hpp>
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Map/String_With_Hash_Unordered_Map.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <map>
#include <memory>
#include <string>

namespace VA {

class AudioBuffer;
class AudioBufferSequenceWithHysteresis;
class AudioEqualizer;
class AudioLowpass;

struct AudioFileInformation {
    std::string filename;
    float gain;
    std::optional<Interval<float>> distance_clamping;
    std::optional<VariableAndHash<std::string>> lowpass;
};

struct AudioMetaInformation {
    float gain;
    std::optional<Interval<float>> distance_clamping;
    std::shared_ptr<AudioLowpass> lowpass;
};

struct AudioFileSequenceInformation {
    std::string filename;
    float gain;
    std::optional<Interval<float>> distance_clamping;
    float hysteresis_step;
};

struct AudioLowpassInformation {
    float gain;
    float gain_hf;
};

struct AudioEqualizerInformation {
    float low_gain;
    float low_cutoff;
    float mid1_gain;
    float mid1_center;
    float mid1_width;
    float mid2_gain;
    float mid2_center;
    float mid2_width;
    float high_gain;
    float high_cutoff;
};

class AudioResources {
    AudioResources(const AudioResources&) = delete;
    AudioResources &operator=(const AudioResources&) = delete;

public:
    AudioResources();
    ~AudioResources();
    void add_buffer(
        const VariableAndHash<std::string>& name,
        const std::string &filename,
        float gain,
        const std::optional<Interval<float>>& distance_clamping,
        const std::optional<VariableAndHash<std::string>>& lowpass);
    const AudioMetaInformation& get_buffer_meta(const VariableAndHash<std::string>& name) const;
    std::shared_ptr<AudioBuffer> get_buffer(const VariableAndHash<std::string>& name) const;
    void preload_buffer(const VariableAndHash<std::string>& name) const;

    void add_buffer_sequence(
        const VariableAndHash<std::string>& name,
        const std::string& filename,
        float gain,
        const std::optional<Interval<float>>& distance_clamping,
        float hysteresis_step);
    float get_buffer_sequence_gain(const VariableAndHash<std::string>& name) const;
    std::shared_ptr<AudioBufferSequenceWithHysteresis>
        get_buffer_sequence(const VariableAndHash<std::string>& name) const;

    void add_equalizer(
        const VariableAndHash<std::string>& name,
        const AudioEqualizerInformation& equalizer) const;
    std::shared_ptr<AudioEqualizer> get_equalizer(const VariableAndHash<std::string>& name) const;

    void add_lowpass(
        const VariableAndHash<std::string>& name,
        const AudioLowpassInformation& lowpass) const;
    std::shared_ptr<AudioLowpass> get_lowpass(const VariableAndHash<std::string>& name) const;
private:
    mutable StringWithHashUnorderedMap<AudioFileInformation> buffer_filenames_;
    mutable StringWithHashUnorderedMap<AudioMetaInformation> buffer_meta_;
    mutable StringWithHashUnorderedMap<std::shared_ptr<AudioBuffer>> buffers_;
    mutable StringWithHashUnorderedMap<AudioFileSequenceInformation> buffer_sequence_filenames_;
    mutable StringWithHashUnorderedMap<std::shared_ptr<AudioBufferSequenceWithHysteresis>>
        buffer_sequences_;
    mutable StringWithHashUnorderedMap<AudioEqualizerInformation> equalizer_parameters_;
    mutable StringWithHashUnorderedMap<std::shared_ptr<AudioEqualizer>> equalizers_;
    mutable StringWithHashUnorderedMap<AudioLowpassInformation> lowpass_parameters_;
    mutable StringWithHashUnorderedMap<std::shared_ptr<AudioLowpass>> lowpasses_;
    mutable SafeAtomicRecursiveSharedMutex mutex_;
};

}
