cmake_minimum_required(VERSION 3.10)

project (http_server)

set(CMAKE_CXX_STANDARD 14)
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(src)

find_package(Boost REQUIRED)
find_package(Threads)

add_library(thread_pool
  src/implements/thread_pool.cc src/include/thread_pool.hpp
)
add_library(lib::tPool ALIAS thread_pool)
target_link_libraries(thread_pool pthread)

add_library(connection
  src/implements/connection.cc src/include/connection.hpp
)
add_library(lib::connection ALIAS connection)

add_subdirectory(tests)
