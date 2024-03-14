cmake_minimum_required(VERSION 3.0.0)
project(linux_internals VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(SOURCE pch.cpp)
list(APPEND SOURCE samples/process.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})


find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})