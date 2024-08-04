#include <iostream>

#include <Server.hpp>
#include <basic.hpp>

int main() {
	ILOG("Server: Execution started");

	LOG("Creating new server instance");
	Server server = Server::host();

	// Connection* conn = server.awaitNewConnection([&](Connection* new_connection) {
	//     ILOG("Acquired new connection on socket " << *new_connection);
	//     return new_connection;
	// });

	// if (conn && conn->isEmpty()) {
	//     delete conn;
	//     return 1;
	// }

	ILOG("Starting connection handling in main");
	server.startConnectionHandling([&](Connection* new_connection) {
		ILOG("Acquired new connection on socket " << *new_connection);
		*new_connection << "Hello, client! from thread";
		return new_connection;
	});
	LOG("Connection handling started");
	std::this_thread::sleep_for(std::chrono::seconds(5));

	// Connection* conn = server.awaitNewConnection([&](Connection* new_connection) {
	//     ILOG("Acquired new connection on socket " << *new_connection);
	// 	*new_connection << "Hello, client!";
	//     return new_connection;
	// });

	// if (conn && conn->isEmpty()) {
	//     delete conn;
	//     return 1;
	// }

	// ILOG("Starting message income handling in main");
	// server.startMessageIncomeHandling([&](std::string message, Connection* connection) {
	// 	ILOG("Received message from connection " << *connection << ": " << message);
	// 	*connection << message;
	// });
	// LOG("Message income handling started");

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