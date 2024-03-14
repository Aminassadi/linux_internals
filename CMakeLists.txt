cmake_minimum_required(VERSION 3.0.0)
project(linux_internals VERSION 0.1.0 LANGUAGES C CXX)

set(SOURCE pch.cpp)
list(APPEND SOURCE samples/process.cpp)


add_executable(${PROJECT_NAME} ${SOURCE})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})