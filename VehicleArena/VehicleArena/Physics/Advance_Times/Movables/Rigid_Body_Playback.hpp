// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Misc/Object.hpp>
#include <VehicleArena/Physics/Interfaces/IAdvance_Time.hpp>
#include <VehicleArena/Physics/Misc/Track_Reader.hpp>
#include <VehicleArena/Scene_Graph/Interfaces/Scene_Node/IAbsolute_Movable.hpp>
#include <chrono>
#include <fstream>
#include <memory>
#include <vector>

namespace VA {

class SceneNode;
class Focuses;

class ITrackElementSequence;
class RigidBodyPlayback;
class CountdownPhysics;

class RigidBodySinglePlayback: public IAbsoluteMovable, public virtual DanglingBaseClass {
    friend RigidBodyPlayback;
public:
    RigidBodySinglePlayback();
    virtual void set_absolute_model_matrix(const TransformationMatrix<float, ScenePos, 3>& absolute_model_matrix) override;
    virtual TransformationMatrix<float, ScenePos, 3> get_new_absolute_model_matrix() const override;
private:
    TransformationMatrix<float, ScenePos, 3> transformation_matrix_;
};

class RigidBodyPlayback: public IAdvanceTime, public virtual DanglingBaseClass {
public:
    RigidBodyPlayback(
        std::unique_ptr<ITrackElementSequence>&& sequence,
        const CountdownPhysics* countdown_start,
        const TransformationMatrix<double, double, 3>* geographic_mapping,
        float speedup,
        size_t ntransformations);
    ~RigidBodyPlayback();
    virtual void advance_time(float dt, const StaticWorld& world) override;
    DanglingBaseClassRef<IAbsoluteMovable> get_playback_object(size_t i);
private:
    const CountdownPhysics* countdown_start_;
    float speedup_;
    double progress_;
    TrackReader track_reader_;
    std::vector<std::unique_ptr<RigidBodySinglePlayback>> playback_objects_;
};

}
