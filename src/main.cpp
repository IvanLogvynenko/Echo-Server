#include <iostream>

#include "Server.hpp"

int main() {
	server_client::Server* server = server_client::Server::host(8080);

	server->startConnectionHandling([&](server_client::Connection *conn) {
		server->sendMessage("Hello, client!", conn);
		return conn;
	});
	server->startMessageIncomeHandling([&](std::string message, server_client::Connection *conn) {
		server->sendMessage("Received: " + message, conn);
	});

	delete server;
}