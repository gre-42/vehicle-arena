// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Rigid_Body_Recorder.hpp"
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Math/Transformation/Tait_Bryan_Angles.hpp>
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Physics/Advance_Times/Countdown_Physics.hpp>
#include <VehicleArena/Physics/Misc/Track_Element.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

RigidBodyRecorder::RigidBodyRecorder(
    const std::string& filename,
    const TransformationMatrix<double, double, 3>* geographic_mapping,
    DanglingBaseClassRef<SceneNode> recorded_node,
    RigidBodyPulses& rbp,
    const CountdownPhysics* countdown_start)
    : countdown_start_{ countdown_start }
    , recorded_node_{ recorded_node.ptr() }
    , rbp_{ &rbp }
    , track_writer_{ filename, geographic_mapping }
    , start_time_{ std::chrono::steady_clock::now() }
{
    recorded_node_->clearing_observers.add({ *this, CURRENT_SOURCE_LOCATION });
}

RigidBodyRecorder::~RigidBodyRecorder() {
    on_destroy.clear();
}

void RigidBodyRecorder::advance_time(float dt, const StaticWorld& world) {
    if (recorded_node_ == nullptr) {
        return;
    }
    if ((countdown_start_ != nullptr) && countdown_start_->counting()) {
        return;
    }
    track_writer_.write(TrackElement{
        .elapsed_seconds = std::chrono::duration<float>{std::chrono::steady_clock::now() - start_time_}.count(),
        .transformations = {UOffsetAndTaitBryanAngles<float, ScenePos, 3>{rbp_->rotation_, rbp_->abs_position()}}});
}

void RigidBodyRecorder::notify_destroyed(SceneNode& destroyed_object) {
    rbp_ = nullptr;
    recorded_node_ = nullptr;

    global_object_pool.remove(this);
}
