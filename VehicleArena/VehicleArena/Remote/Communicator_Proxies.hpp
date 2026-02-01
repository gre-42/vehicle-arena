// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_List.hpp>
#include <VehicleArena/Memory/Dangling_Unordered_Set.hpp>
#include <VehicleArena/Memory/Dangling_Value_Unordered_Map.hpp>
#include <VehicleArena/Memory/Destruction_Notifier.hpp>
#include <VehicleArena/Remote/Remote_Site_Id.hpp>
#include <compare>
#include <iosfwd>
#include <memory>

namespace VA {

class ISendSocket;
class ICommunicatorProxy;
class IReceiveSocket;

using ReceiveSockets = DanglingUnorderedSet<IReceiveSocket>;
using HandshakeProxies = DanglingList<ICommunicatorProxy>;
using CommunicatorProxyMap = DanglingValueUnorderedMap<RemoteSiteId, ICommunicatorProxy>;

enum class TransmissionType {
    HANDSHAKE,
    UNICAST,
    MULTICAST
};

class ICommunicatorProxy: public virtual DestructionNotifier, public virtual DanglingBaseClass {
public:
    virtual ~ICommunicatorProxy() = default;
    virtual void set_send_socket(std::shared_ptr<ISendSocket> send_socket) = 0;
    virtual void receive_from_home(std::istream& istr) = 0;
    virtual void send_home(std::iostream& iostr) = 0;
};

class ICommunicatorProxyFactory: public virtual DestructionNotifier, public virtual DanglingBaseClass {
public:
    virtual DanglingBaseClassRef<ICommunicatorProxy> create_handshake_proxy(
        std::shared_ptr<ISendSocket> send_socket) = 0;
    virtual DanglingBaseClassRef<ICommunicatorProxy> create_communicator_proxy(
        std::shared_ptr<ISendSocket> send_socket,
        RemoteSiteId home_site_id) = 0;
};

class CommunicatorProxies {
public:
    CommunicatorProxies(
        const DanglingBaseClassRef<ICommunicatorProxyFactory>& communicator_proxy_factory,
        RemoteSiteId site_id);
    ~CommunicatorProxies();
    void add_receive_socket(const DanglingBaseClassRef<IReceiveSocket>& socket);
    void add_handshake_socket(std::shared_ptr<ISendSocket> socket);
    void send_and_receive(TransmissionType transmission_type);
    void print(std::ostream& ostr) const;
private:
    void send(TransmissionType transmission_type);
    void receive();
    ReceiveSockets receive_sockets_;
    HandshakeProxies handshake_communicator_proxies_;
    CommunicatorProxyMap unicast_communicator_proxies_;
    CommunicatorProxyMap multicast_communicator_proxies_;
    DanglingBaseClassRef<ICommunicatorProxyFactory> communicator_proxy_factory_;
    RemoteSiteId site_id_;
};

std::ostream& operator << (std::ostream& ostr, const CommunicatorProxies& distributed_system);

}
