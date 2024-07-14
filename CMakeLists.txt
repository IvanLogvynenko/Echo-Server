cmake_minimum_required(VERSION 3.5)
project(Echo-Server)

find_package(debugKit REQUIRED)
find_package(ServerClientLib REQUIRED)
    
add_executable(server
    src/Server/main.cpp
)
target_link_libraries(server PRIVATE ServerClientLib)

add_executable(client
    src/Client/main.cpp
)
target_link_libraries(client PRIVATE ServerClientLib)
