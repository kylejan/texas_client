#pragma once

#include "texas_code/protocol/raw_message.h"
#include "texas_code/protocol/message.pb.h"
#include "texas_code/protocol/message_type.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <deque>
#include <string>
#include <condition_variable>

#include <spdlog/spdlog.h>
#include <boost/asio/io_service.hpp>

namespace texas_code {
namespace client {

using namespace protocol;

class Messenger {
public:
    Messenger(const std::string& rpc_endpoint, const std::string& sub_endpoint);

    Messenger(const Messenger&) = delete;
    Messenger(Messenger&&) = delete;
    Messenger& operator = (const Messenger&) = delete;
    Messenger& operator = (Messenger&&) = delete;

    boost::asio::io_service& get_service();
    void send_message(MessageType, const ::google::protobuf::Message&);

    virtual void init();
    virtual void run();
    virtual void dispatch_message(std::unique_ptr<InquireActionMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<SeatInfoMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<BlindMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<FlopMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<HoldCardsMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<TurnMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<RiverMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<InquireShowDownMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<ShowDownMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<ShowDownResponse>) = 0;
    virtual void dispatch_message(std::unique_ptr<PotWinMessage>) = 0;
    virtual void dispatch_message(std::unique_ptr<ConnectResponse>) = 0;

private:
    template <typename type>
    void dispatch(std::unique_ptr<RawMessage> raw_message) {
        std::unique_ptr<type> message(new type());
        message->ParseFromString(raw_message->msg_body);
        spdlog::get("console")->info("receive message {0}\n{1}", raw_message->msg_type, message->Utf8DebugString());
        dispatch_message(std::move(message));
    }

    void socket_rpc_send_and_recv(RawMessage*);
    void socket_sub_recv();
    void handle_recv_message();

    zmq::context_t context_;
    zmq::socket_t rpc_socket_;
    zmq::socket_t sub_socket_;

    std::string rpc_endpoint_;
    std::string sub_endpoint_;

    std::deque<std::unique_ptr<RawMessage>> queue_;
    boost::asio::io_service io_service_;
    boost::asio::io_service::work work_;
};

} // namespace client
} // namespace texas_code
