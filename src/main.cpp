#include "texas_code/client/messenger.h"

int main() {
    using namespace texas_code::client;
    using namespace texas_code::protocol;

    auto* messenger = new texas_code::client::Messenger("tcp://localhost:5555", "tcp://localhost:5556");

    ConnectRequest req;
    req.set_request_id(1);
    req.set_account_name("test");

    RawMessage message(static_cast<int>(MessageType::CONNECT_REQUEST), req.SerializeAsString());

    messenger->socket_rpc_send(&message);

    return 0;
}
