cmake_minimum_required(VERSION 3.2)

file(GLOB_RECURSE NET_SOURCES "src/*.h" "src/*.cpp")
add_library(net STATIC ${NET_SOURCES})
target_link_libraries(net
    PRIVATE ws2_32
)

add_executable(client client.cpp)
target_link_libraries(client
    PRIVATE net
)

add_executable(server server.cpp)
target_link_libraries(server
    PRIVATE net
)
