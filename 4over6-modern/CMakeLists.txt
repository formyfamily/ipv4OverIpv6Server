cmake_minimum_required(VERSION 3.7)
project(4over6_modern)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
find_package (Boost COMPONENTS system filesystem thread REQUIRED)
include_directories (${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp tcp_connection.cpp tcp_connection.h ip_pool.cpp ip_pool.h tools.cpp tools.h tun_helper.cpp tun_helper.h tcp_server.cpp tcp_server.h msg_queue.cpp msg_queue.h)
add_executable(4over6_modern ${SOURCE_FILES})
target_link_libraries(4over6_modern PUBLIC ${Boost_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
