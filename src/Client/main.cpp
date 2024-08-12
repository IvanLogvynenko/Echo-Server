#include <iostream>

#include "Client.hpp"

int main() {
	server_client::Client client;

	std::cout << "Client started." << std::endl;
	client.connectTo("127.0.0.1", 8080);
	std::cout << client.recieve() << std::endl;
	std::cout << "Connected to server." << std::endl;
	client.sendMessage("Hello, server!");
	std::cout << client.recieve() << std::endl;
	client.disconnect();
	std::cout << "Disconnected from server." << std::endl;
}