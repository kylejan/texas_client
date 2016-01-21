#pragma once

#include "texas_code/protocol/raw_message.h"
#include "texas_code/protocol/message.pb.h"
#include "texas_code/protocol/message_type.h"

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <string>

#include <iostream>

namespace texas_code {
namespace client {

using namespace protocol;

class Messenger {
public:
    Messenger(const std::string& rpc_endpoint, const std::string& sub_endpoint)
        : rpc_endpoint_{rpc_endpoint}
        , sub_endpoint_{sub_endpoint}
        , context_{1}
        , rpc_socket_{context_, ZMQ_REQ}
        , sub_socket_{context_, ZMQ_SUB} {

        rpc_socket_.connect(rpc_endpoint_);
        sub_socket_.connect(sub_endpoint);
    }

    void socket_rpc_send(RawMessage* raw_message) {
        zmq::message_t message = raw_message->pack_zmq_msg();
        rpc_socket_.send(message);
        zmq::message_t reply;
        rpc_socket_.recv(&reply);
    }

    void socket_sub_receive(RawMessage* raw_message) {
        zmq::message_t message;
        sub_socket_.recv(&message);
    }

private:
    zmq::context_t context_;
    zmq::socket_t rpc_socket_;
    zmq::socket_t sub_socket_;

    std::string rpc_endpoint_;
    std::string sub_endpoint_;
};

} // namespace client
} // namespace texas_code
