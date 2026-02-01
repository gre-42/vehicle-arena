// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Remote/IReceive_Socket.hpp>
#include <VehicleArena/Remote/ISend_Socket.hpp>
#include <VehicleArena/Remote/Sockets/Asio.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <VehicleArena/Threads/J_Thread.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

namespace VA {

struct ReceivedMessage {
    std::vector<std::byte> message;
    std::unique_ptr<ISendSocket> reply_socket;
};

class UdpNode: public ISendSocket, public IReceiveSocket {
public:
    UdpNode(
        const std::string& ip_address,
        uint16_t port);
    UdpNode(
        std::shared_ptr<boost::asio::io_context> io_context,
        std::shared_ptr<boost::asio::ip::udp::socket> socket,
        boost::asio::ip::udp::endpoint endpoint,
        size_t max_stored_received_messages);
    ~UdpNode();
    void start_receive_thread(size_t max_stored_received_messages);
    void bind();
    void shutdown();
    virtual void send(std::istream& istr) override;
    virtual std::unique_ptr<ISendSocket> try_receive(std::ostream& ostr) override;
private:
    std::shared_ptr<boost::asio::io_context> io_context_;
    std::shared_ptr<boost::asio::ip::udp::socket> socket_;
    boost::asio::ip::udp::endpoint endpoint_;
    FastMutex message_mutex_;
    std::jthread receive_thread_;
    std::list<ReceivedMessage> messages_received_;
};

}
