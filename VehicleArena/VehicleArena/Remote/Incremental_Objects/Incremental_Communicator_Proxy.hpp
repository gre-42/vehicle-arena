// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Remote/Communicator_Proxies.hpp>
#include <VehicleArena/Remote/Incremental_Objects/Incremental_Remote_Objects.hpp>
#include <VehicleArena/Remote/Incremental_Objects/Known_Fields.hpp>
#include <VehicleArena/Remote/Incremental_Objects/Scene_Level.hpp>
#include <VehicleArena/Remote/Remote_Site_Id.hpp>
#include <iosfwd>
#include <unordered_map>

namespace VA {

class ISendSocket;
class IIncrementalObjectFactory;
enum class IoVerbosity;
enum class ProxyTasks;

class IncrementalCommunicatorProxy final: public virtual ICommunicatorProxy {
public:
    IncrementalCommunicatorProxy(
        std::shared_ptr<ISendSocket> send_socket,
        const DanglingBaseClassRef<IIncrementalObjectFactory>& shared_object_factory,
        const DanglingBaseClassRef<IncrementalRemoteObjects>& objects,
        IoVerbosity verbosity,
        ProxyTasks tasks,
        RemoteSiteId home_site_id);
    virtual ~IncrementalCommunicatorProxy() override;
    virtual void set_send_socket(std::shared_ptr<ISendSocket> send_socket) override;
    virtual void receive_from_home(std::istream& istr) override;
    virtual void send_home(std::iostream& iostr) override;
private:
    std::unordered_map<RemoteObjectId, KnownFields> known_fields_;
    std::shared_ptr<ISendSocket> send_socket_;
    DanglingBaseClassRef<IIncrementalObjectFactory> shared_object_factory_;
    DanglingBaseClassRef<IncrementalRemoteObjects> objects_;
    IoVerbosity verbosity_;
    ProxyTasks tasks_;
    RemoteSiteId home_site_id_;
    std::optional<LocalSceneLevel> home_scene_level_;
};

}
