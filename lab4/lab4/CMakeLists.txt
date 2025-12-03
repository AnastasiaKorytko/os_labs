cmake_minimum_required(VERSION 3.14)
project(Lab4_MessageQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(message_queue STATIC
    message_queue.cpp
    receiver.cpp
    sender.cpp
    message_queue.h
    receiver.h
    sender.h
)

target_include_directories(message_queue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(message_queue PRIVATE /W4 /permissive-)
else()
    target_compile_options(message_queue PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(receiver_main main_receiver.cpp)
target_link_libraries(receiver_main PRIVATE message_queue)

add_executable(sender_main main_sender.cpp)
target_link_libraries(sender_main PRIVATE message_queue)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp)
target_link_libraries(tests PRIVATE message_queue gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

add_custom_target(run_tests
    COMMAND tests
    DEPENDS tests
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)