cmake_minimum_required(VERSION 3.10)

project (http_server)

set(CMAKE_CXX_STANDARD 14)
set(ROOT ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(src)

find_package(Boost REQUIRED COMPONENTS filesystem)

add_library(thread_pool
  src/implements/thread_pool.cc src/include/thread_pool.hpp
)
add_library(lib::tPool ALIAS thread_pool)
target_link_libraries(thread_pool pthread)

add_library(http_utils
  src/implements/http_utils.cc src/include/http_utils.hpp
)
add_library(lib::http_utils ALIAS http_utils)

add_library(connection
  src/implements/connection.cc src/include/connection.hpp
)
target_link_libraries(connection PUBLIC lib::http_utils)
add_library(lib::connection ALIAS connection)


add_library(http_server
  src/implements/http_server.cc src/include/http_server.hpp
)
target_link_libraries(http_server PUBLIC lib::connection)
add_library(lib::http_server ALIAS http_server)

add_executable(main src/main.cc)
target_link_libraries(main PUBLIC
  lib::tPool
  lib::connection
  lib::http_server
  lib::http_utils
  ${Boost_LIBRARIES}
)

add_subdirectory(tests)
