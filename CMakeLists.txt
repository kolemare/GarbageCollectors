cmake_minimum_required(VERSION 3.10)

project(GarbageCollector)

# Set C++ standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable testing
enable_testing()

# Add subdirectories for Google Test and Graphviz
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/application/include/googletest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/application/include/graphviz)

# Set output directory for built binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/application/bin)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/application/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/application/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/application/include/graphviz/lib) # Include Graphviz submodule headers

# Add source files for the application
set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/application/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/GarbageCollector.hpp
)

# Add executable for the application
add_executable(application ${APP_SOURCES})

# Link the Google Test library and Graphviz libraries to the application
target_link_libraries(application gvplugin_core gvplugin_dot_layout gvplugin_neato_layout cgraph gvc)

# Add source files for tests
set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/testGarbageCollector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/application/src/GarbageCollector.hpp
)

# Add executable for tests
add_executable(tests ${TEST_SOURCES})

# Link the Google Test library and Graphviz libraries to the tests
target_link_libraries(tests gtest gtest_main gvplugin_core gvplugin_dot_layout gvplugin_neato_layout cgraph gvc)

# Set output directory for tests executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# Add test
add_test(NAME tests COMMAND tests)