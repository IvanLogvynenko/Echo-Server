cmake_minimum_required(VERSION 3.5)
project(Echo-Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Werror -fsanitize=address")

# include("packages/ServerClientLib.cmake")
    
add_executable(server
    src/Server/main.cpp

    src/Server.cpp
    src/Client.cpp
    src/Connection.cpp
)
target_include_directories(server PRIVATE include/)
target_compile_definitions(server PRIVATE EXECUTABLE_NAME="SERVER")
# target_compile_definitions(server PRIVATE FULL_LOGGING)
target_compile_definitions(server PRIVATE LOGGING)
target_compile_definitions(server PRIVATE ERROR)
# target_link_libraries(server PRIVATE ServerClientLib)


add_executable(client
    src/Client/main.cpp

    src/Server.cpp
    src/Client.cpp
    src/Connection.cpp
)
# target_link_libraries(client PRIVATE ServerClientLib)
target_include_directories(client PRIVATE include/)
target_compile_definitions(client PRIVATE EXECUTABLE_NAME="CLIENT")
# target_compile_definitions(client PRIVATE FULL_LOGGING)
target_compile_definitions(client PRIVATE LOGGING)
target_compile_definitions(client PRIVATE ERROR)
