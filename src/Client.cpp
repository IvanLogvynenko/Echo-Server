#include "Client.hpp"

server_client::Client& Client(Client &other) {
	if (this == &other)
		return other;

	this->connection = other.connection;
}

server_client::Client& operator=(Client& other) {
	if (this == &other)
		return other;

	this->connection = other.connection;
}

virtual int server_client::openSocket() {

}

void server_client::connect(std::string ip, std::string port) {
	struct addrinfo hints;
	struct addrinfo* res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int socketd = 0;

	if (int status = getaddrinfo(host, std::to_string(port).c_str(), &hints, &res); status != 0) {
		freeaddrinfo(res);
		throw std::runtime_error("Getaddrinfo error: " + std::string(gai_strerror(status)));
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd == -1) {
		freeaddrinfo(res);
		throw std::runtime_error("Socket error " + std::string(strerror(errno)));
	}

	int attempt = 0;
	while (attempt++ < MAX_CONNECTION_ATTEMPTS && connect(socket_fd, res->ai_addr, res->ai_addrlen) == -1)
		std::cerr << "Connection failed | attempt " << attempt << std::endl;
	
	if (attempt == MAX_CONNECTION_ATTEMPTS) {
		freeaddrinfo(res);
		throw std::runtime_error("Impossible to connect to server");
	}
	
	Connection connection = new Connection(socket_fd);
	m_connection = connection;
	freeaddrinfo(res);

}
void server_client::connect(std::string ip, int port) {
	this->connect(ip, std::to_string(port);	
}

void server_client::disconnect() {
	delete this->connection;
	this->connection = nullptr;
}

server_client::~Client() {
	if (this->connection)
		delete this->connection;
{


