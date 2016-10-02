cmake_minimum_required(VERSION 3.0)
project(loquat-test)

option(COLLECT_COVERAGE "Collects code coverage with gcov" Off)

add_subdirectory(googletest)

include_directories(${gtest_SOURCE_DIR}/include)
include_directories(../include)

file(GLOB TEST_SOURCES
     "main.cpp"
     "container/*.cpp"
     "math/*.cpp")

set(COMPILE_OPTIONS "-std=c++11 -Wall -Wextra")
set(LINK_OPTIONS    " ")

if(COLLECT_COVERAGE)
	set(COMPILE_OPTIONS "${COMPILE_OPTIONS} -O0 -g -coverage")
	set(LINK_OPTIONS    "${LINK_OPTIONS} -O2 -g -coverage")
endif()

add_executable(loquat-test ${TEST_SOURCES})
set_target_properties(loquat-test PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS})
set_target_properties(loquat-test PROPERTIES LINK_FLAGS    ${LINK_OPTIONS})
target_link_libraries(loquat-test gtest gcov)

