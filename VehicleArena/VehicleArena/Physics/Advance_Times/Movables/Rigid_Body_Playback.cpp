// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Rigid_Body_Playback.hpp"
#include <VehicleArena/Geometry/Coordinates/Homogeneous.hpp>
#include <VehicleArena/Macro_Executor/Focus.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Physics/Advance_Times/Countdown_Physics.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

RigidBodyPlayback::RigidBodyPlayback(
    std::unique_ptr<ITrackElementSequence>&& sequence,
    const CountdownPhysics* countdown_start,
    const TransformationMatrix<double, double, 3>* geographic_mapping,
    float speedup,
    size_t ntransformations)
    : countdown_start_{countdown_start}
    , speedup_{speedup}
    , progress_{0.}
    , track_reader_{
        std::move(sequence),
        0, // nframes
        1, // nlaps
        geographic_mapping,
        TrackElementInterpolationKey::ELAPSED_SECONDS,
        TrackReaderInterpolationMode::LINEAR,
        ntransformations
    }
{
    playback_objects_.resize(ntransformations);
    for (auto& o : playback_objects_) {
        o = std::make_unique<RigidBodySinglePlayback>();
    }
}

RigidBodyPlayback::~RigidBodyPlayback() {
    on_destroy.clear();
}

void RigidBodyPlayback::advance_time(float dt, const StaticWorld& world) {
    if ((countdown_start_ != nullptr) && countdown_start_->counting()) {
        return;
    }
    if (track_reader_.read(progress_)) {
        progress_ += dt / seconds * speedup_;
        const auto& t = track_reader_.track_element().element.transformations;
        if (t.size() != playback_objects_.size()) {
            throw std::runtime_error("Conflicting playback sizees");
        }
        for (size_t i = 0; i < t.size(); ++i) {
            playback_objects_[i]->transformation_matrix_ = t[i].to_matrix();
        }
    }
}

DanglingBaseClassRef<IAbsoluteMovable> RigidBodyPlayback::get_playback_object(size_t i) {
    if (i >= playback_objects_.size()) {
        throw std::runtime_error("Playback-object index out of bounds");
    }
    return { *playback_objects_[i], CURRENT_SOURCE_LOCATION };
}

RigidBodySinglePlayback::RigidBodySinglePlayback()
    : transformation_matrix_{ fixed_nans<float, 3, 3>(), fixed_nans<ScenePos, 3>() }
{}

void RigidBodySinglePlayback::set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) {
    transformation_matrix_ = absolute_model_matrix;
}

TransformationMatrix<float, ScenePos, 3> RigidBodySinglePlayback::get_new_absolute_model_matrix() const {
    return transformation_matrix_;
}
