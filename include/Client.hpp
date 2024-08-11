#ifndef CLIENT_HPP
#define CLIENT_HPP

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1000
#endif // !BUFFER_SIZE

#ifndef MAX_CONNECTION_ATTEMPTS
#define MAX_CONNECTION_ATTEMPTS 50
#endif // !MAX_CONNECTION_ATTEMPTS

#ifndef DEFAULT_PORT
#define DEFAULT_PORT "30000"
#endif // !DEFAULT_PORT

#include <string>
#include <cstddef>

#include <sys/socket.h>

#include "Connection.hpp"

namespace server_client {
	class Client {
		protected: 
			Connection* connection;
			virtual int openSocket();
		public:
			//Client();
			Client(Client&);
			Client& operator=(Client&);
			~Client();

			// I/O
			void sendMessage(const char *);
			void sendmessage(std::string);

			std::string recieve();

			// net controll
			void connect(std::string, std::string = DEFAULT_PORT);
			void connect(std::string, uint16_t);
			
			void disconnect();
			
			inline operator int() { return *connection; }
	}
}
