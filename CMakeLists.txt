cmake_minimum_required(VERSION 3.5)
project(Echo-Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

add_library(ServerClientLib
	src/Server.cpp
	src/Connection.cpp
)
target_include_directories(ServerClientLib PRIVATE "include/")

add_executable(
	server
	src/main.cpp
)
target_link_libraries(server PRIVATE ServerClientLib)
target_include_directories(server PRIVATE "include/")

