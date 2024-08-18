cmake_minimum_required(VERSION 3.5)
project(Echo-Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Wconversion -Werror -fsanitize=address")

# include("references/ServerClientLib.cmake")

include("references/debugKit.cmake")
add_library(ServerClientLib1
    src/Server.cpp
    src/Client.cpp
    src/Connection.cpp
)
target_link_libraries(ServerClientLib1 PUBLIC debugKit)
target_include_directories(ServerClientLib1 PUBLIC include)

add_executable(
	server
	src/Server/main.cpp
)
target_compile_definitions(server PRIVATE EXECUTABLE_NAME="Server")
target_link_libraries(server PRIVATE ServerClientLib1)
target_link_libraries(server PRIVATE debugKit)
target_include_directories(server PRIVATE include)

add_executable(
	client
	src/Client/main.cpp
)
target_compile_definitions(client PRIVATE EXECUTABLE_NAME="Client")
target_link_libraries(client PRIVATE ServerClientLib1)
target_link_libraries(client PRIVATE debugKit)
target_include_directories(client PRIVATE include)
