// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Track_Reader.hpp"
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Physics/Misc/ITrack_Element_Sequence.hpp>
#include <VehicleArena/Testing/Assert.hpp>
#include <stdexcept>

using namespace VA;

TrackReader::TrackReader(
    std::unique_ptr<ITrackElementSequence>&& sequence,
    size_t nframes,
    size_t nlaps,
    const TransformationMatrix<double, double, 3>* inverse_geographic_mapping,
    TrackElementInterpolationKey interpolation_key,
    TrackReaderInterpolationMode interpolation_mode,
    size_t ntransformations)
    : sequence_{std::move(sequence)}
    , frame_id_{0}
    , lap_id_{0}
    , nframes_remaining_{nframes}
    , nlaps_remaining_{nlaps}
    , inverse_geographic_mapping_{inverse_geographic_mapping}
    , interpolation_key_{interpolation_key}
    , interpolation_mode_{interpolation_mode}
    , ntransformations_{ntransformations}
    , track_element0_{std::nullopt}
    , track_element1_{std::nullopt}
{
    if (finished()) {
        throw std::runtime_error("Number of frames or number of laps must be at least 1");
    }
}

TrackReader::~TrackReader() = default;

bool TrackReader::read(double& progress) {
    if (inverse_geographic_mapping_ == nullptr) {
        throw std::runtime_error("TrackReader::read without geographic mapping");
    }
    history_.clear();
    if (!sequence_->eof()) {
        while (!finished() && (!track_element1_.has_value() || (track_element1_->progress(interpolation_key_) < progress)))
        {
            if (track_element1_.has_value()) {
                track_element0_ = track_element1_;
            }
            track_element1_ = sequence_->read(track_element1_, *inverse_geographic_mapping_, ntransformations_);
            if ((nframes_remaining_ != SIZE_MAX) && (nlaps_remaining_ == 0)) {
                --nframes_remaining_;
            }
            if (sequence_->eof()) {
                if (finished()) {
                    return false;
                } else {
                    if ((nlaps_remaining_ != SIZE_MAX) && (nlaps_remaining_ > 0)) {
                        ++lap_id_;
                        --nlaps_remaining_;
                    }
                    if (finished()) {
                        return false;
                    }
                    if (!track_element1_.has_value()) {
                        throw std::runtime_error("Received empty and periodic track");
                    }
                    // This assumes that the last element of the track equals the first element,
                    // i.e. that the start the track looks like [0 1 2 3 4 0].
                    // Note that "nlaps_remaining_" is not reset.
                    sequence_->restart();
                    progress = 0.;
                    track_element0_ = std::nullopt;
                    track_element1_ = std::nullopt;
                    frame_id_ = 0;
                    continue;
                }
            } else {
                ++frame_id_;
            }
            if (track_element1_.has_value()) {
                history_.push_back(*track_element1_);
            }
            if (!track_element0_.has_value()) {
                track_element0_ = track_element1_;
            }
        }
        if (!track_element0_.has_value() || !track_element1_.has_value()) {
            verbose_abort("Internal error in TrackReader::read");
        }
        if (track_element1_->progress(interpolation_key_) == track_element0_->progress(interpolation_key_)) {
            track_element_ = track_element0_.value();
        } else {
            float alpha = float(
                (progress - track_element0_->progress(interpolation_key_)) /
                (track_element1_->progress(interpolation_key_) - track_element0_->progress(interpolation_key_)));
            assert_true(alpha >= 0);
            assert_true(alpha <= 1);
            if (interpolation_mode_ == TrackReaderInterpolationMode::NEAREST_NEIGHBOR) {
                alpha = std::round(alpha);
            } else if (interpolation_mode_ != TrackReaderInterpolationMode::LINEAR) {
                throw std::runtime_error("Unknown track-reader interpolation-mode");
            }
            track_element_ = interpolated(*track_element0_, *track_element1_, alpha);
        }
        return true;
    }
    return false;
}

bool TrackReader::finished() const {
    return (nframes_remaining_ == 0) && (nlaps_remaining_ == 0);
}
