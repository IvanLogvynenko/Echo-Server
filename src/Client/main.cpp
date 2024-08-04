#include <iostream>

#include "Client.hpp"

int main() {
    std::cout << "Client: Execution started" << std::endl;

    Client client = Client();
    client.connectTo(); // << std::string("Hello, world!");

    client.sendMessage("Hello, Server!");
    std::string data = client.recieveMessage();
    ILOG("Received message: " << data);

    return 0;
}