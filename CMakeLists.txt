cmake_minimum_required(VERSION 3.5)
project(Echo-Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

add_library(ServerClientLib
	src/Server.cpp
	src/Connection.cpp
	src/Client.cpp
)
target_include_directories(ServerClientLib PRIVATE include)

add_executable(
	server
	src/Server/main.cpp
)
target_link_libraries(server PRIVATE ServerClientLib)
target_include_directories(server PRIVATE include)

add_executable(
	client
	src/Client/main.cpp
)
target_link_libraries(client PRIVATE ServerClientLib)
target_include_directories(client PRIVATE include)
