cmake_minimum_required(VERSION 4.0.0)


project(test)


include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test test.cpp ../src/core/ipc/unnamedPipe.cpp)

target_include_directories(test INTERFACE ../src/core/ipc)

target_link_libraries(test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test)