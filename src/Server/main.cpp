#include <iostream>

#include <Server.hpp>
#include <basic.hpp>

int main() {
    ILOG("Server: Execution started");

    Server server = Server::host();
    server.startConnectionHandling([&](Connection* new_connection) {
        ILOG("Acquired new connection on socket " << *new_connection);
        return new_connection;
    });

    server.startMessageIncomeHandling([&](std::string message, Connection* connection) {
        ILOG("Received message from connection " << *connection << ": " << message);
        *connection << message;
    });

    // Connection* conn = server.awaitNewConnection([&](Connection* new_connection) {
    //     ILOG("Acquired new connection on socket " << *new_connection);
    //     return new_connection;
    // });

    // if (conn && conn->isEmpty())
    //     delete conn;

    // server.awaitNewMessage(conn, [&](std::string message, Connection* connection) {
    //     ILOG("Received message from connection " << *connection << ": " << message);
    //     *connection << message;
    // });

    return 0;
}