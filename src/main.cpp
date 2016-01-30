#include "texas_code/client/client.h"

int main() {
    auto* client = new texas_code::client::Client("tcp://localhost:5555", "tcp://localhost:5556");
    client->init();
    client->run();
    return 0;
}
