// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Audio_Buffer.hpp"
#include <VehicleArena/Audio/Alut_Init_Without_Context.hpp>
#include <VehicleArena/Audio/CHK.hpp>
#include <VehicleArena/Memory/Integral_Cast.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <mutex>
#include <stdexcept>
#include <vector>

using namespace VA;

AudioBuffer::AudioBuffer(ALuint handle)
    : handle_{ handle }
{
    // AL_CHK(alGenBuffers((ALuint)1, &handle_));
}

AudioBuffer::~AudioBuffer() {
    AL_ABORT(alDeleteBuffers((ALuint)1, &handle_));
}

// static ALenum to_al_format(short channels, short samples)
// {
//     bool stereo = (channels > 1);
// 
//     switch (samples) {
//         case 16:
//             if (stereo)
//                 return AL_FORMAT_STEREO16;
//             else
//                 return AL_FORMAT_MONO16;
//         case 8:
//             if (stereo)
//                 return AL_FORMAT_STEREO8;
//             else
//                 return AL_FORMAT_MONO8;
//         default:
//             throw std::runtime_error("Unsupported samples");
//     }
// }

std::shared_ptr<AudioBuffer> AudioBuffer::from_wave(const std::string& filename) {
    // WaveInfo* wave = WaveOpenFileForReading(filename.c_str());
    // if (wave == nullptr) {
    //     throw std::runtime_error("Failed to read wave file: \"" + filename + '"');
    // }

    // try {
    //     {
    //         int ret = WaveSeekFile(0, wave);
    //         if (ret != 0) {
    //             throw std::runtime_error("Failed to seek wave file: \"" + filename + '"');
    //         }
    //     }

    //     std::vector<char> bufferData(wave->dataSize);
    //     {
    //         int ret = WaveReadFile(bufferData.data(), wave->dataSize, wave);
    //         if (ret != (int)wave->dataSize) {
    //             throw std::runtime_error(
    //                 "Short read: " + std::to_string(ret) +
    //                 ", want: " + std::to_string(wave->dataSize) +
    //                 ", file: \"" + filename + '"');
    //         }
    //     }

    //     AL_CHK(alBufferData(
    //         buffer_, to_al_format(wave->channels, wave->bitsPerSample),
    //         bufferData.data(), wave->dataSize, wave->sampleRate));
    // } catch (...) {
    //     WaveCloseFile(wave);
    //     throw;
    // }
    // WaveCloseFile(wave);

    AlutInitWithoutContext alut_init_without_context;
    // ALuint buffer = alutCreateBufferFromFile(filename.c_str());
    auto data = read_file_bytes(filename);
    ALuint buffer = alutCreateBufferFromFileImage(data.data(), integral_cast<ALsizei>(data.size()));
    if (buffer == AL_NONE) {
        ALenum error = alutGetError();
        throw std::runtime_error("Could not load file \"" + filename + "\": " + alutGetErrorString(error) + ", code " + std::to_string(error));
    }
    return std::make_shared<AudioBuffer>(buffer);
}

uint32_t AudioBuffer::nchannels() const {
    ALint value;
    AL_CHK(alGetBufferi(
        handle_,
        AL_CHANNELS,
        &value));
    return integral_cast<uint32_t>(value);
}
