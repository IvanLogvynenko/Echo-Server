#include <iostream>

#include "Server.hpp"

#include "Logger.hpp"
#include "LoggerConfig.hpp"

int main() {
	LoggerConfig::getInstance()
	    // ->logToFile("server.log")
	    ->setExecutableName("SERVER");
	Logger logger;
	server_client::Server* server = server_client::Server::host(8080);
	logger << Logger::important << "Server started on port 8080" << std::endl;

	server->startConnectionHandling([&](server_client::Connection *conn) {
		server->sendMessage("Hello, client!", conn);
		return conn;
	});
	logger << "Server is listening for incoming connections..." << std::endl;
	server->startMessageIncomeHandling([&](std::string message, server_client::Connection *conn) {
		server->sendMessage("Received: " + message, conn);
	});
	logger << "Server is responding to incoming messages..." << std::endl;

	delete server;
	logger << Logger::important << "Server stopped" << std::endl;
	return 0;
}
