#include <iostream>

#include "Server.hpp"

#include "Logger.hpp"
#include "LoggerConfig.hpp"

int main() {
	LoggerConfig::getInstance()->setExecutableName("SERVER");

	Logger logger;
	logger << Logger::important << "Starting server..." << std::endl;
	Server* server = Server::host(8080);
	logger << Logger::important << "Server started on port 8080" << std::endl;

	server->startConnectionHandling([&](const Connection *conn) {
		conn->sendMessage("Hello, client!");
	});
	logger << "Server is listening for incoming connections..." << std::endl;
	
	server->startMessageIncomeHandling([&](const std::string message, const Connection *conn) {
		conn->sendMessage("Received: " + message);
	});
	logger << "Server is responding to incoming messages..." << std::endl;

	delete server;
	logger << Logger::important << "Server stopped" << std::endl;
	Logger::flush();
	return 0;
}
