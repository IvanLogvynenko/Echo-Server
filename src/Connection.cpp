#include "Connection.hpp"

using namespace server_client;

unsigned int Connection::ID = 0;
const std::string Connection::CLOSE_MESSAGE = "SYSTEM_MESSAGE<CONNECTION CLOSED>\n";

Connection::Connection(int sd) : 
	sd(sd), 
	id(ID++),
	backlog("") {}
Connection::Connection(const Connection& other) : 
    sd(other.sd), 
    id(other.id),
	backlog(other.backlog) {}
Connection& Connection::operator=(const Connection& other) {
	if (&other == this) 
		return *this;

	this->id = other.id;
	this->sd = other.sd;
	this->backlog = other.backlog;
	return *this;
}



server_client::Connection::~Connection() {
	#include "stdio.h"
	printf("Closing connection ID: %u\n", this->id);
	close(this->sd);
}
