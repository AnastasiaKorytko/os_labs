cmake_minimum_required(VERSION 3.14)
project(MarkerLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(marker_system STATIC marker_system.cpp marker_system.h)
target_include_directories(marker_system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(marker_main main.cpp)
target_link_libraries(marker_main PRIVATE marker_system)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE marker_system gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
