#include <iostream>

#include <Server.hpp>

int main() {
    std::cout << "Server: Execution started" << std::endl;

    Server server{DEFAULT_PORT};

    server.awaitNewConnection();

    server.startConnectionHandling([&](Connection& new_connection) {
        ILOG("Acquired new connection on socket " << new_connection);
    });

    

    return 0;
}