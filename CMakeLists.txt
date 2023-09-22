cmake_minimum_required(VERSION 3.26)
project(Client_Server_Sockets C)

set(CMAKE_C_STANDARD 11)

add_executable(server src/server.c)
add_executable(client src/client.c)

target_link_libraries(server pthread)

include_directories(include)
