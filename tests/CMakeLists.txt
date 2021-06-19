include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  thread_pool_test
  thread_pool.cc
)

target_include_directories(thread_pool_test PUBLIC ${ROOT}/src)

target_link_libraries(
  thread_pool_test
  lib::tPool
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(thread_pool_test)
