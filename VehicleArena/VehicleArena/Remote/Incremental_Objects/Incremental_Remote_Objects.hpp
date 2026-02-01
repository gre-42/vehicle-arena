// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Value_Map.hpp>
#include <VehicleArena/Memory/Destruction_Notifier.hpp>
#include <VehicleArena/Remote/Events/Events_And_Times.hpp>
#include <VehicleArena/Remote/Incremental_Objects/Remote_Object_Id.hpp>
#include <VehicleArena/Time/Time_And_Pause.hpp>
#include <chrono>
#include <iosfwd>
#include <map>

namespace VA {

class IIncrementalObject;
class SceneLevelSelector;

using DeletedObjects = EventsAndTimes<RemoteObjectId, std::chrono::steady_clock::time_point>;
using LocalObjects = DanglingValueMap<LocalObjectId, IIncrementalObject>;
using RemoteObjects = DanglingValueMap<RemoteObjectId, IIncrementalObject>;

enum class RemoteObjectVisibility {
    PRIVATE,
    PUBLIC
};

class IncrementalRemoteObjects: public virtual DestructionNotifier, public virtual DanglingBaseClass {
public:
    explicit IncrementalRemoteObjects(
        RemoteSiteId local_site_id,
        const DanglingBaseClassRef<SceneLevelSelector>& local_scene_level_selector);
    ~IncrementalRemoteObjects();
    RemoteSiteId local_site_id() const;
    std::chrono::steady_clock::time_point local_time() const;
    PauseStatus pause_status() const;
    void set_local_time(const TimeAndPause<std::chrono::steady_clock::time_point>& time);
    DanglingBaseClassRef<SceneLevelSelector> local_scene_level_selector() const;
    RemoteObjectId add_local_object(
        const DanglingBaseClassRef<IIncrementalObject>& object,
        RemoteObjectVisibility visibility);
    void add_remote_object(
        const RemoteObjectId& id,
        const DanglingBaseClassRef<IIncrementalObject>& object,
        RemoteObjectVisibility visibility);
    DanglingBaseClassPtr<IIncrementalObject> try_get(const RemoteObjectId& id) const;
    bool try_remove(const RemoteObjectId& id);
    const DeletedObjects& deleted_objects() const;
    void forget_old_deleted_objects();
    const LocalObjects& private_local_objects() const;
    const LocalObjects& public_local_objects() const;
    const RemoteObjects& public_remote_objects() const;
    void print(std::ostream& ostr) const;

private:
    RemoteSiteId local_site_id_;
    TimeAndPause<std::chrono::steady_clock::time_point> local_time_;
    DanglingBaseClassRef<SceneLevelSelector> local_scene_level_selector_;
    DeletedObjects deleted_objects_;
    LocalObjectId next_local_object_id_;
    LocalObjects private_local_objects_;
    LocalObjects public_local_objects_;
    RemoteObjects private_remote_objects_;
    RemoteObjects public_remote_objects_;
};

std::ostream& operator << (std::ostream& ostr, const IncrementalRemoteObjects& objects);

}
