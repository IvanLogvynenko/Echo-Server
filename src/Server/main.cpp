#include <iostream>

#include "Server.hpp"

#include "Logger.hpp"
#include "LoggerConfig.hpp"

int main() {
	LoggerConfig::getInstance()
	    // ->logToFile("server.log")
	    ->setExecutableName("SERVER");
	Logger logger;
	Server* server = Server::host(8080);
	logger << Logger::important << "Server started on port 8080" << std::endl;

	server->startConnectionHandling([&](const Connection *conn) {
		conn->sendMessage("Hello, client!");
		// return conn;
	});
	logger << "Server is listening for incoming connections..." << std::endl;
	server->startMessageIncomeHandling([&](const std::string message, const Connection *conn) {
		conn->sendMessage("Received: " + message);
	});
	logger << "Server is responding to incoming messages..." << std::endl;

	delete server;
	logger << Logger::important << "Server stopped" << std::endl;
	return 0;
}
