cmake_minimum_required(VERSION 3.5)
project(Echo-Server)

set(CMAKE_CXX_STANDARD 20)

include("references/debugKit.cmake")
include("references/ServerClientLib.cmake")

add_executable(
	server
	src/Server/main.cpp
)
target_link_libraries(server PRIVATE ServerClientLib)
target_link_libraries(server PRIVATE debugKit)
target_include_directories(server PRIVATE include)

add_executable(
	client
	src/Client/main.cpp
)
target_link_libraries(client PRIVATE ServerClientLib)
target_link_libraries(client PRIVATE debugKit)
target_include_directories(client PRIVATE include)
