#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <cstdint>

#include <stdexcept>

#include <array>
#include <string>

#include <atomic>
#include <thread>
#include <functional>

#include <unistd.h>
#include <sys/socket.h>

#include "Logger.hpp"
	
namespace server_client
{
	class Connection {
		private:
			static unsigned int ID;
		protected:
			int sd;
			unsigned int id;

			std::function<void(const std::string&)> on_message;
			std::atomic<std::thread*> message_handler;
		public:
			Connection(int sd);
			Connection(const Connection&);
			~Connection();
			Connection& operator=(const Connection&);

			std::string awaitNewMessage();
			std::string recieve();

			void sendMessage(const std::string& message);

			inline unsigned int getId() const { return this->id; }
			inline int getSocket() const { return this->sd; }
			inline operator int() const { return this->sd; }
			inline bool isValid() const { return this->sd != -1 && this->sd != 0; }

			const static std::string CLOSE_MESSAGE;
			const static std::string SERVER_STOP;
	};

	// class ConnectionHashFunction {
	// public:
	// 	size_t operator() (const server_client::Connection& connection) const {
	// 		return std::hash<int>{}(connection.getSocket());
	// 	}
	// };


	class ConnectionClosedException : public std::exception {
	public:
		ConnectionClosedException() {}
		virtual const char* what() const noexcept override { return ""; }
	};

} // namespace server_client

#endif //!CONNECTION_HPP
