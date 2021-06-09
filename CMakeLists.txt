cmake_minimum_required(VERSION 3.10)

project (http_server)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/include)

find_package(Boost REQUIRED)
find_package(Threads)

add_library(thread_pool
  src/implements/thread_pool.cc src/include/thread_pool.hpp
)
add_library(lib::tPool ALIAS thread_pool)

target_link_libraries(thread_pool pthread)

target_include_directories(thread_pool PUBLIC
  src
)

add_subdirectory(tests)
