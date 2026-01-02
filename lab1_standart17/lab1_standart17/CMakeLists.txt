cmake_minimum_required(VERSION 3.10)
project(lab1cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(lab1_functions functions.cpp)
target_include_directories(lab1_functions PRIVATE ${CMAKE_SOURCE_DIR})

add_executable(Creator Creator.cpp)
target_include_directories(Creator PRIVATE ${CMAKE_SOURCE_DIR})
add_executable(Reporter Reporter.cpp)
target_include_directories(Reporter PRIVATE ${CMAKE_SOURCE_DIR})
add_executable(Main Main.cpp functions.cpp)
target_include_directories(Main PRIVATE ${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(UnitTests tests.cpp functions.cpp)
target_include_directories(UnitTests PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(UnitTests gtest_main)

include(GoogleTest)
gtest_discover_tests(UnitTests)