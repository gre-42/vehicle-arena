// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Map/Verbose_Unordered_Map.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <VehicleArena/Signal/Exponential_Smoother.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <chrono>
#include <iosfwd>
#include <optional>
#include <unordered_map>

namespace VA {

class AudioSource;
template <class TPosition>
struct AudioSourceState;
enum class AudioDistanceModel;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;

struct AudioSourceNode {
    ExponentialSmoother<FixedArray<float, 3>, float> relative_position;
    ExponentialSmoother<FixedArray<float, 3>, float> relative_velocity;
};

class AudioScene {
    AudioScene() = delete;
    AudioScene(const AudioScene&) = delete;
    AudioScene &operator=(const AudioScene&) = delete;
public:
    static void set_default_alpha(float alpha);
    static void add_source(AudioSource& source, float alpha);
    static void remove_source(AudioSource& source);
    static void set_listener(
        const TransformationMatrix<float, ScenePos, 3>& pose,
        const FixedArray<float, 3>& velocity);
    static void set_source_transformation(
        AudioSource& source,
        const AudioSourceState<ScenePos>& state);
    static void set_distance_model(AudioDistanceModel model);
    static void print(std::ostream& ostr);
private:
    static FastMutex mutex_;
    static float default_alpha_;
    static VerboseUnorderedMap<const AudioSource*, AudioSourceNode> source_nodes_;
};

}
