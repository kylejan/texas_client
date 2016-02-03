#include "texas_code/client/client.h"

#include <spdlog/spdlog.h>

int main() {
    size_t q_size = 1048576;
    spdlog::set_async_mode(q_size);

    auto* client = new texas_code::client::Client("tcp://localhost:5555", "tcp://localhost:5556");
    client->init();
    client->run();
    return 0;
}
