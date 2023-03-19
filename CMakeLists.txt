cmake_minimum_required(VERSION 3.10)

project(GarbageCollector)

# set C++ standard

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/application/include/googletest)

# set output directory for built binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/application/bin)

# include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/application/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/application/src)

# add source files for the application
set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/application/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/GarbageCollector.hpp
)

# add executable for the application
add_executable(application ${APP_SOURCES})

# add source files for tests
set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/testGarbageCollector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/GarbageCollector.hpp
)

# add executable for tests
add_executable(tests ${TEST_SOURCES})

# link the Google Test library to the tests
target_link_libraries(tests gtest gtest_main)

# set output directory for tests executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# add test
add_test(NAME tests COMMAND tests)