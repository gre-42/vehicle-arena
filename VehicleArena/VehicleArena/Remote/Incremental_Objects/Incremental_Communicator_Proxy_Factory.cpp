// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Incremental_Communicator_Proxy_Factory.hpp"
#include <VehicleArena/Remote/Incremental_Objects/IIncremental_Object_Factory.hpp>
#include <VehicleArena/Remote/Incremental_Objects/Incremental_Communicator_Proxy.hpp>
#include <VehicleArena/Remote/Incremental_Objects/Incremental_Remote_Objects.hpp>
#include <VehicleArena/Remote/Incremental_Objects/Proxy_Tasks.hpp>

using namespace VA;

IncrementalCommunicatorProxyFactory::IncrementalCommunicatorProxyFactory(
    const DanglingBaseClassRef<IIncrementalObjectFactory>& shared_object_factory,
    const DanglingBaseClassRef<IncrementalRemoteObjects>& objects,
    IoVerbosity verbosity,
    ProxyTasks tasks)
    : shared_object_factory_{ shared_object_factory }
    , objects_{ objects }
    , verbosity_{ verbosity }
    , tasks_{ tasks }
    , object_pool_{ InObjectPoolDestructor::CLEAR }
{}

IncrementalCommunicatorProxyFactory::~IncrementalCommunicatorProxyFactory() = default;

DanglingBaseClassRef<ICommunicatorProxy> IncrementalCommunicatorProxyFactory::create_handshake_proxy(
    std::shared_ptr<ISendSocket> send_socket)
{
    return { object_pool_.create<IncrementalCommunicatorProxy>(
            CURRENT_SOURCE_LOCATION,
            std::move(send_socket),
            shared_object_factory_,
            objects_,
            verbosity_,
            ProxyTasks::NONE,
            0xC0FEFACE),
        CURRENT_SOURCE_LOCATION
    };
}

DanglingBaseClassRef<ICommunicatorProxy> IncrementalCommunicatorProxyFactory::create_communicator_proxy(
    std::shared_ptr<ISendSocket> send_socket,
    RemoteSiteId home_site_id)
{
    return { object_pool_.create<IncrementalCommunicatorProxy>(
            CURRENT_SOURCE_LOCATION,
            std::move(send_socket),
            shared_object_factory_,
            objects_,
            verbosity_,
            tasks_,
            home_site_id),
        CURRENT_SOURCE_LOCATION
    };
}
