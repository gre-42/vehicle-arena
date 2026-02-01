// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Communicator_Proxies.hpp"
#include <VehicleArena/Io/Binary.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Remote/IReceive_Socket.hpp>
#include <VehicleArena/Remote/ISend_Socket.hpp>

using namespace VA;

CommunicatorProxies::CommunicatorProxies(
    const DanglingBaseClassRef<ICommunicatorProxyFactory>& communicator_proxy_factory,
    RemoteSiteId site_id)
    : communicator_proxy_factory_{ communicator_proxy_factory }
    , site_id_{ site_id }
{}

CommunicatorProxies::~CommunicatorProxies() = default;

void CommunicatorProxies::add_receive_socket(const DanglingBaseClassRef<IReceiveSocket>& socket) {
    receive_sockets_.emplace(socket.ptr(), CURRENT_SOURCE_LOCATION);
}

void CommunicatorProxies::add_handshake_socket(std::shared_ptr<ISendSocket> socket)
{
    auto proxy = communicator_proxy_factory_->create_handshake_proxy(std::move(socket));
    handshake_communicator_proxies_.emplace_back(proxy, proxy.loc());
}

void CommunicatorProxies::send_and_receive(TransmissionType transmission_type) {
    send(transmission_type);
    receive();
}

void CommunicatorProxies::send(TransmissionType transmission_type) {
    switch (transmission_type) {
    case TransmissionType::HANDSHAKE:
        for (auto& proxy : handshake_communicator_proxies_) {
            std::stringstream sstr;
            write_binary(sstr, site_id_, "location ID");
            proxy->send_home(sstr);
        }
        return;
    case TransmissionType::UNICAST:
        for (auto& [_, proxy] : unicast_communicator_proxies_) {
            std::stringstream sstr;
            write_binary(sstr, site_id_, "location ID");
            proxy->send_home(sstr);
        }
        return;
    case TransmissionType::MULTICAST:
        for (auto& [_, proxy] : multicast_communicator_proxies_) {
            std::stringstream sstr;
            write_binary(sstr, site_id_, "location ID");
            proxy->send_home(sstr);
        }
        return;
    }
    throw std::runtime_error("Unknown transmission type");
}

void CommunicatorProxies::receive() {
    for (auto& s : receive_sockets_) {
        while (true) {
            std::stringstream sstr;
            auto responder = s->try_receive(sstr);
            if (responder == nullptr) {
                break;
            }
            auto communicator_proxy = [&](){
                auto site_id = read_binary<RemoteSiteId>(sstr, "node ID", IoVerbosity::SILENT);
                auto it = unicast_communicator_proxies_.find(site_id);
                if (it == unicast_communicator_proxies_.end()) {
                    auto f = communicator_proxy_factory_->create_communicator_proxy(std::move(responder), site_id);
                    auto res = unicast_communicator_proxies_.emplace(site_id, std::move(f), CURRENT_SOURCE_LOCATION);
                    if (!res.second) {
                        verbose_abort("Could not add communicator proxy with ID \"" + std::to_string(site_id) + '"');
                    }
                    return res.first->second.object();
                } else {
                    it->second->set_send_socket(std::move(responder));
                    return it->second.object();
                }
            }();
            // linfo() << "Receive at location " << location_id_;
            communicator_proxy->receive_from_home(sstr);
        }
    }
}

void CommunicatorProxies::print(std::ostream& ostr) const {
    ostr <<
        "#receive_sockets: " << receive_sockets_.size() <<
        " #handshake: " << handshake_communicator_proxies_.size() <<
        " #unicast: " << unicast_communicator_proxies_.size() <<
        " #multicast: " << multicast_communicator_proxies_.size();
}

std::ostream& VA::operator << (std::ostream& ostr, const CommunicatorProxies& distributed_system) {
    distributed_system.print(ostr);
    return ostr;
}
