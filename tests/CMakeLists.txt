enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(thread_pool_test thread_pool.test.cc)
target_include_directories(thread_pool_test PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(thread_pool_test
  thread_pool
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(thread_pool_test)