// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Object_Pool.hpp>
#include <VehicleArena/Remote/Communicator_Proxies.hpp>

namespace VA {

class IIncrementalObjectFactory;
class IncrementalRemoteObjects;
enum class IoVerbosity;
enum class ProxyTasks;

class IncrementalCommunicatorProxyFactory: public ICommunicatorProxyFactory {
public:
    explicit IncrementalCommunicatorProxyFactory(
        const DanglingBaseClassRef<IIncrementalObjectFactory>& shared_object_factory,
        const DanglingBaseClassRef<IncrementalRemoteObjects>& objects,
        IoVerbosity verbosity,
        ProxyTasks tasks);
    virtual ~IncrementalCommunicatorProxyFactory() override;
    virtual DanglingBaseClassRef<ICommunicatorProxy> create_handshake_proxy(
        std::shared_ptr<ISendSocket> send_socket) override;
    virtual DanglingBaseClassRef<ICommunicatorProxy> create_communicator_proxy(
        std::shared_ptr<ISendSocket> send_socket,
        RemoteSiteId home_site_id) override;
private:
    DanglingBaseClassRef<IIncrementalObjectFactory> shared_object_factory_;
    DanglingBaseClassRef<IncrementalRemoteObjects> objects_;
    IoVerbosity verbosity_;
    ProxyTasks tasks_;
    ObjectPool object_pool_;
};

}
