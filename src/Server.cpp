#include "Server.hpp"

Server::Server() : m_socket(0), m_port(0), m_connection_handler(nullptr) {}

Server::Server(Server &other)
{
    if (this == &other) 
		return;

	this->m_socket = other.m_socket;
	this->m_port = other.m_port;

	//making it in the separate zone to automatically deallocate locks
	{
	std::lock_guard<std::mutex> lock(m_connections_lock);
	std::lock_guard<std::mutex> other_lock(other.m_connections_lock);
	this->m_connections.clear();
	this->m_connections = other.m_connections;
	}

	this->m_on_connect = other.m_on_connect;
    this->m_on_recieve = other.m_on_recieve;

	this->m_message_income_threads = other.m_message_income_threads;
}
Server &Server::operator=(Server &other)
{
	//yeah it is copy paste. Fuck dry
    if (this == &other) 
		return *this;

	this->m_socket = other.m_socket;
	this->m_port = other.m_port;

	//making it in the separate zone to automatically deallocate locks
	{
	std::lock_guard<std::mutex> lock(m_connections_lock);
	std::lock_guard<std::mutex> other_lock(other.m_connections_lock);
	this->m_connections.clear();
	this->m_connections = other.m_connections;
	}

	this->m_on_connect = other.m_on_connect;
    this->m_on_recieve = other.m_on_recieve;

	this->m_message_income_threads = other.m_message_income_threads;

    return *this;
}

//main thread method implementations
std::thread* Server::startNewMessageIncomeThread(Connection* connection) {
	return std::move( new std::thread([this, &connection] () {

		pollfd data = (pollfd) { *connection, POLLIN, 0 };

		while (this->m_handle_message_income) {
			int poll_res = poll(&data, 1, HANDLING_TIMEOUT);
			
			std::string message;
			if (poll_res == -1) {
				EL("Error occurred while recieving. Continuing...");
				continue;
			}
			else if (poll_res == 0) {
				LOG("Timeout. Continuing...");
				m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection));
				this->m_message_income_threads.erase(*connection);
				break;
			}
			
			try {
				*connection >> message;
			} catch (std::exception& e) {
				//in case of any error we close a connection, 
				//stop a thread and remove connection from list
				m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection));
				std::lock_guard<std::mutex> lock(this->m_message_income_threads_lock);
				this->m_message_income_threads.erase(*connection);
				break;
			}

			if (this->m_on_recieve)
				this->m_on_recieve(message, connection);
		}

	}));
}

void Server::startEventHandler()
{
	// if (m_connection_handler.load())
	// 	return;

	// m_connection_handler.store( std::move( new std::thread( [&]() {

	// 	ILOG("Starting main thread");

	// 	while(!this->m_thread_stop.load()) {
	// 		if (!this->m_handle_connection) {
	// 			std::unique_lock<std::mutex> lock(this->m_main_thread_mutex);
	// 			this->m_main_thread_lock.wait(lock);
	// 		}

	// 		Connection* connection = this->awaitNewConnection();
	// 		if (connection->isEmpty()) 
	// 			delete connection;
			
	// 		if (!this->m_handle_message_income)
	// 			continue;

	// 		std::lock_guard<std::mutex> lock(this->m_message_income_threads_lock);
	// 		this->m_message_income_threads[*connection] = startNewMessageIncomeThread(connection);
	// 	}



	// 	delete this->m_connection_handler.load();
	// })));
}
void Server::stopEventHandler()
{
	// if (!m_connection_handler.load())
    //     return;

    // ILOG("Stopping main thread");
	// this->m_thread_stop.store(true);
	// m_main_thread_lock.notify_one();
    // m_connection_handler.load()->join();
    // m_connection_handler.store(nullptr);
}

// * configuration methods
Server Server::host(uint16_t port)
{
    Server server{};
    int socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (socket_fd == -1) {
		EL("Socket error");
		throw std::runtime_error("Socket opening error");
	}

	struct sockaddr_in socketAddr;
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr = INADDR_ANY;

	int yes = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

	if (bind(socket_fd, (struct sockaddr*)&socketAddr, sizeof(socketAddr))) {
		EL("Bind error");
		close(socket_fd);
		throw std::runtime_error("Caught error while binding");
	}

	if (listen(socket_fd, LISTEN_BACKLOG)) {
		EL("Listen error");
		close(socket_fd);
		throw std::runtime_error("Failed to set socket to listening state");
	}

	server.m_port = port;
	server.m_socket = socket_fd;
	ILOG("Server started on port " << port);
	LOG("Current socket " << socket_fd);
	return server;
}

Connection *Server::awaitNewConnection(std::function<Connection *(Connection *)> on_connect)
{
	pollfd data = (pollfd) { this->m_socket, POLLIN, 0 };
	LOG("Connection timeout is " << HANDLING_TIMEOUT << "ms");
	int poll_res = poll(&data, 1, HANDLING_TIMEOUT);

	if (poll_res == -1) {
		EL("Error occurred while connecting. Dropping...");
		return new Connection(-1);
	}
	else if (poll_res == 0) {
		LOG("Timeout. Exiting...");
		return new Connection(-1);
	}
	
	int new_fd = accept(this->m_socket, nullptr, 0);
	if (!new_fd) {
		EL("Failed to accept new connection. Skipping...");
	}

	ILOG("New connection accepted");
	Connection* connection = new Connection(new_fd);

	if (on_connect)
		this->m_on_connect = std::move(on_connect);

	if (this->m_on_connect)
		connection = this->m_on_connect(connection);

	if (!connection->isEmpty())
		this->m_connections.push_back(connection);

	LOG("Connection proccessed and added to the list if not empty");
	return connection;
}
std::string Server::awaitNewMessage(
	Connection *connection, std::function<void(std::string &, Connection *)> on_recieve)
{
	pollfd data = (pollfd) { *connection, POLLIN, 0 };
	LOG("Awaiting for new message. Timeout is " << HANDLING_TIMEOUT << "ms");
	int poll_res = poll(&data, 1, HANDLING_TIMEOUT);
	
	std::string message;
	if (poll_res == -1) {
		EL("Error occurred while recieving. Exiting...");
		exit(EXIT_FAILURE);
	}
	else if (poll_res == 0) {
		LOG("Timeout. Continuing...");
		return message;
	}
	
	LOG("Got new message. Processing...");
	*connection >> message;
	LOG("Message: " << message);

	if (on_recieve)
		this->m_on_recieve = std::move(on_recieve);

	if (this->m_on_recieve)
		this->m_on_recieve(message, connection);
	
	return message;
}

void Server::startConnectionHandling(std::function<Connection* (Connection *)> on_connect)
{	
	if (m_handle_connection)
		return;

	if (on_connect)
		this->m_on_connect = std::move(on_connect);

	m_handle_connection = true;
	// m_main_thread_lock.notify_one();
	
	LOG("Starting connection handling");
	m_connection_handler.store( std::move( new std::thread( [&]() {

		ILOG("Starting connection handling thread");
		
		while(this->m_handle_connection) {
			// if (!this->m_handle_connection) {
			// 	std::unique_lock<std::mutex> lock(this->m_main_thread_mutex);
			// 	this->m_main_thread_lock.wait(lock);
			// }
			LOG("Awaiting for new connection. Timeout is " << HANDLING_TIMEOUT << "ms");
			Connection* connection = this->awaitNewConnection();
			if (connection->isEmpty()) 
				delete connection;
			
			if (!this->m_handle_message_income)
				continue;

			std::lock_guard<std::mutex> lock(this->m_message_income_threads_lock);
			this->m_message_income_threads[*connection] = startNewMessageIncomeThread(connection);
		}
		LOG("Connection handling thread stopped");
	} ) ) );

	// this->startEventHandler();
}
void Server::stopConnectionHandling()
{
	m_handle_connection = false;
	LOG("Deleting connection handling thread " << m_connection_handler);
	if (m_connection_handler) {
		m_connection_handler.load()->join();
		delete this->m_connection_handler;
	}
}

void Server::startMessageIncomeHandling(std::function<void(std::string &, Connection *)> on_recieve)
{
	LOG("Starting message income handling");
	if (m_handle_message_income)
		return;

	if (on_recieve)
		this->m_on_recieve = std::move(on_recieve);
	
	// starting threads for incoming connections
	m_handle_message_income = true;

	//starting threads for existing connections
	std::lock_guard<std::mutex> lock(this->m_message_income_threads_lock);
	for(auto connection : this->m_connections)
		m_message_income_threads.insert_or_assign(*connection, startNewMessageIncomeThread(connection));
}
void Server::stopMessageIncomeHandling()
{
	m_handle_message_income = false;
	std::lock_guard<std::mutex> lock(this->m_message_income_threads_lock);
	this->m_message_income_threads.clear();
}

// * I/O methods
void Server::sendMessage(const char * data, size_t index)
{
	try {
		sendMessage((std::string)data, *this->m_connections[index]);
	}
	catch (std::runtime_error &re) {
		std::lock_guard<std::mutex> lock(m_connections_lock);
		delete this->m_connections[index];
		this->m_connections.erase(this->m_connections.begin() + index);
	}
}
void Server::sendMessage(std::string data, size_t index)
{
	try {
		sendMessage((std::string)data, *this->m_connections[index]);
	}
	catch (std::runtime_error &re) {
		std::lock_guard<std::mutex> lock(m_connections_lock);
		delete this->m_connections[index];
		this->m_connections.erase(this->m_connections.begin() + index);
	}
}
void Server::sendMessage(std::string data, const Connection & connection)
{
	connection << data;
}

std::string Server::recieveMessageFrom(size_t index)
{
	return recieveMessageFrom(*this->m_connections[index]);
}
std::string Server::recieveMessageFrom(const Connection &connection)
{
	m_lastly_used_connection = connection;
	return connection.recieve();
}

void Server::respond(const char * data) const
{
	respond((std::string)data);
}
void Server::respond(std::string data) const
{
	if(this->m_lastly_used_connection == Connection::empty) {
		EL("You cannot respond since you have not received anything");
		throw std::runtime_error("You cannot respond since you have not received anything");
	}
	LOG("Sending message: " << data);
    m_lastly_used_connection << data;
}

// getters
Server::operator int() const
{
    return m_socket;
}
int Server::getPort() const
{
    return this->m_port;
}
std::vector<Connection*> Server::getConnections() const
{
    return this->m_connections;
}

Server::~Server()
{
	this->stopMessageIncomeHandling();
	this->stopConnectionHandling();
	// this->stopEventHandler();
	LOG("Threads stopped. Deleting connections");

	std::lock_guard<std::mutex> lock(this->m_connections_lock);
	for (auto &connection : this->m_connections)
		delete connection;
}
