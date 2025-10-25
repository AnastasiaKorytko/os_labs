cmake_minimum_required(VERSION 3.14)
project(N3_linkedlist LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(N3_linkedlist N3_linkedlist.cpp)
target_include_directories(N3_linkedlist PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(main_program main.cpp)
target_link_libraries(main_program PRIVATE N3_linkedlist)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE N3_linkedlist gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
