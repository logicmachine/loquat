cmake_minimum_required(VERSION 3.0)
project(loquat-test)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include)
include_directories(../include)

file(GLOB TEST_SOURCES
     "main.cpp"
     "container/*.cpp"
     "math/*.cpp")

set(COMPILE_OPTIONS "-std=c++11 -Wall -Wextra")
set(LINK_OPTIONS    " ")

add_executable(loquat-test ${TEST_SOURCES})
set_target_properties(loquat-test PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS})
set_target_properties(loquat-test PROPERTIES LINK_FLAGS    ${LINK_OPTIONS})
target_link_libraries(loquat-test gtest)

