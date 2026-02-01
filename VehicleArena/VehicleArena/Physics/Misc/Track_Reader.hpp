// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Misc/Track_Element_Extended.hpp>
#include <optional>

namespace VA {

template <class TDir, class TPos, size_t n>
class TransformationMatrix;
class ITrackElementSequence;
enum class TrackReaderInterpolationMode {
    NEAREST_NEIGHBOR,
    LINEAR
};

class TrackReader {
public:
    explicit TrackReader(
        std::unique_ptr<ITrackElementSequence>&& sequence,
        size_t nframes,
        size_t nlaps,
        const TransformationMatrix<double, double, 3>* inverse_geographic_mapping,
        TrackElementInterpolationKey interpolation_key,
        TrackReaderInterpolationMode interpolation_mode,
        size_t ntransformations);
    ~TrackReader();
    bool read(double& progress);
    bool finished() const;
    inline const TrackElementExtended& track_element() const {
        return track_element_;
    }
    inline std::list<TrackElementExtended>& history() {
        return history_;
    }
    inline size_t frame_id() const {
        return frame_id_;
    }
    inline size_t lap_id() const {
        return lap_id_;
    }
    inline double progress() const {
        if (!has_value()) {
            throw std::runtime_error("TrackReader::progress called on uninitialized object");
        }
        return track_element_.progress(interpolation_key_);
    }
    inline bool has_value() const {
        return track_element0_.has_value() && track_element1_.has_value();
    }
private:
    std::unique_ptr<ITrackElementSequence> sequence_;
    TrackElementExtended track_element_;
    std::list<TrackElementExtended> history_;
    size_t frame_id_;
    size_t lap_id_;
    size_t nframes_remaining_;
    size_t nlaps_remaining_;
    const TransformationMatrix<double, double, 3>* inverse_geographic_mapping_;
    TrackElementInterpolationKey interpolation_key_;
    TrackReaderInterpolationMode interpolation_mode_;
    size_t ntransformations_;
    std::optional<TrackElementExtended> track_element0_;
    std::optional<TrackElementExtended> track_element1_;
};

}
