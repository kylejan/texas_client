#include "texas_code/client/client.h"

#include <spdlog/spdlog.h>

namespace texas_code {

namespace client {

Client::Client(const std::string& rpc_endpoint, const std::string& sub_endpoint)
    : Messenger(rpc_endpoint, sub_endpoint) {
}

void Client::init() {
    Messenger::init();
    get_service().post([this]{
        Heartbeat request;
        request.set_sequence_num(time(nullptr));
        spdlog::get("console")->info(request.sequence_num());
        send_message(static_cast<std::int32_t>(MessageType::HEARTBEAT), request.SerializeAsString());
    });
}

void Client::run() {
    Messenger::run();
}

void Client::dispatch_message(std::unique_ptr<InquireActionMessage>) {

}

void Client::dispatch_message(std::unique_ptr<SeatInfoMessage>) {

}

void Client::dispatch_message(std::unique_ptr<BlindMessage>) {

}

void Client::dispatch_message(std::unique_ptr<FlopMessage>) {

}

void Client::dispatch_message(std::unique_ptr<HoldCardsMessage>) {

}

void Client::dispatch_message(std::unique_ptr<TurnMessage>) {

}

void Client::dispatch_message(std::unique_ptr<RiverMessage>) {

}

void Client::dispatch_message(std::unique_ptr<InquireShowDownMessage>) {

}

void Client::dispatch_message(std::unique_ptr<ShowDownMessage>) {

}

void Client::dispatch_message(std::unique_ptr<ShowDownResponse>) {

}

void Client::dispatch_message(std::unique_ptr<PotWinMessage>) {

}

void Client::dispatch_message(std::unique_ptr<ConnectResponse>) {

}

}
}
