#include "texas_code/client/messenger.h"

namespace texas_code {

namespace client {

Messenger::Messenger(const std::string& rpc_endpoint, const std::string& sub_endpoint)
    : rpc_endpoint_{rpc_endpoint}
    , sub_endpoint_{sub_endpoint}
    , context_{1}
    , rpc_socket_{context_, ZMQ_REQ}
    , sub_socket_{context_, ZMQ_SUB}
    , work_(io_service_) {

    rpc_socket_.connect(rpc_endpoint_);
    sub_socket_.connect(sub_endpoint);
}

void Messenger::init() {
    get_service().post([this]{
        while (true) {
            auto raw_message = socket_sub_recv();
            handle_recv_message(std::move(raw_message));
        }
    });
}

void Messenger::run() {
    get_service().run();
}

boost::asio::io_service& Messenger::get_service() {
    return io_service_;
}

std::unique_ptr<RawMessage> Messenger::send_and_recv(std::int32_t msg_type, const std::string& msg_body) {
    std::unique_ptr<RawMessage> raw_message(new RawMessage(msg_type, msg_body));
    auto response = socket_rpc_send_and_recv(std::move(raw_message));
    return std::move(response);
}

std::unique_ptr<RawMessage> Messenger::socket_sub_recv() {
    zmq::message_t message;
    sub_socket_.recv(&message);
    std::unique_ptr<RawMessage> response(new RawMessage(&message));
    return std::move(response);
}

std::unique_ptr<RawMessage> Messenger::socket_rpc_send_and_recv(std::unique_ptr<RawMessage> raw_message) {
    zmq::message_t message = raw_message->pack_zmq_msg();
    rpc_socket_.send(message);
    zmq::message_t reply;
    rpc_socket_.recv(&reply);
    std::unique_ptr<RawMessage> response(new RawMessage(&message));
    return std::move(response);
}

void Messenger::handle_recv_message(std::unique_ptr<RawMessage> raw_message) {
    if (raw_message == nullptr) return;

    switch (static_cast<MessageType>(raw_message->msg_type)) {
        case MessageType::UNKNOWN_REQUEST:
            return;
        case MessageType::INQUIRE_ACTION_MESSAGE:
            dispatch<InquireActionMessage>(std::move(raw_message));
            break;
        case MessageType::SEAT_INFO_MESSAGE:
            dispatch<SeatInfoMessage>(std::move(raw_message));
            break;
        case MessageType::BLIND_MESSAGE:
            dispatch<BlindMessage>(std::move(raw_message));
            break;
        case MessageType::FLOP_MESSAGE:
            dispatch<FlopMessage>(std::move(raw_message));
            break;
        case MessageType::HOLD_CARDS_MESSAGE:
            dispatch<HoldCardsMessage>(std::move(raw_message));
            break;
        case MessageType::TURN_MESSAGE:
            dispatch<TurnMessage>(std::move(raw_message));
            break;
        case MessageType::RIVER_MESSAGE:
            dispatch<RiverMessage>(std::move(raw_message));
            break;
        case MessageType::INQUIRE_SHOW_DOWN_MESSAGE:
            dispatch<InquireShowDownMessage>(std::move(raw_message));
            break;
        case MessageType::SHOW_DOWN_MESSAGE:
            dispatch<ShowDownMessage>(std::move(raw_message));
            break;
        case MessageType::SHOW_DOWN_RESPONSE:
            dispatch<ShowDownResponse>(std::move(raw_message));
            break;
        case MessageType::POT_WIN_MESSAGE:
            dispatch<PotWinMessage>(std::move(raw_message));
            break;
        case MessageType::CONNECT_RESPONSE:
            dispatch<ConnectResponse>(std::move(raw_message));
            break;
        default:
            break;
    }
}

}
}