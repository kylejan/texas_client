#pragma once

#include "texas_code/client/messenger.h"

namespace texas_code {

namespace client {

class Client : public Messenger {
public:
    Client(const std::string&, const std::string&);

    Client(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator = (const Client&) = delete;
    Client& operator = (Client&&) = delete;

    void init();
    void run();
    void dispatch_message(std::unique_ptr<InquireActionMessage>);
    void dispatch_message(std::unique_ptr<SeatInfoMessage>);
    void dispatch_message(std::unique_ptr<BlindMessage>);
    void dispatch_message(std::unique_ptr<FlopMessage>);
    void dispatch_message(std::unique_ptr<HoldCardsMessage>);
    void dispatch_message(std::unique_ptr<TurnMessage>);
    void dispatch_message(std::unique_ptr<RiverMessage>);
    void dispatch_message(std::unique_ptr<InquireShowDownMessage>);
    void dispatch_message(std::unique_ptr<ShowDownMessage>);
    void dispatch_message(std::unique_ptr<ShowDownResponse>);
    void dispatch_message(std::unique_ptr<PotWinMessage>);
    void dispatch_message(std::unique_ptr<ConnectResponse>);

};

} // namespace client
} // namespace texas_code
