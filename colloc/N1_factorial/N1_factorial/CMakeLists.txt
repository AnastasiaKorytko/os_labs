cmake_minimum_required(VERSION 3.14)
project(FactorialLib LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(factorial_lib N1_factorial.cpp)
target_include_directories(factorial_lib PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(N1_factorial main.cpp)
target_link_libraries(N1_factorial PRIVATE factorial_lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE factorial_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)