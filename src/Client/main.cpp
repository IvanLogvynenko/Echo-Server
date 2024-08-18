#include <iostream>

#include "Client.hpp"
#include "Logger.hpp"
#include "LoggerConfig.hpp"
// #include "LogLevel.hpp"

int main() {
	Logger logger;
	LoggerConfig::getInstance()
		// ->ignoreLevels({LogLevel::DEBUG})
		// ->logToFile("client.log")
		->setExecutableName("CLIENT");
	server_client::Client client;

	logger << "Client started." << std::endl;
	client.connectTo("127.0.0.1", 8080);
	logger << "Connected to server." << std::endl;
	logger << client.recieve() << std::endl;
	client.sendMessage("Hello, server!");
	logger << client.recieve() << std::endl;
	client.disconnect();
	logger << "Disconnected from server." << std::endl;
	logger << "end" << std::endl;
}