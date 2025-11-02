cmake_minimum_required(VERSION 3.20)

project(JSON_PARSER
    VERSION 1.0
    DESCRIPTION "Json Parser"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/std:c++latest)
endif()

# Include your local headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect your source files
file(GLOB_RECURSE JSON_PARSER_SOURCES "src/*.cpp")

# Define your executable
add_executable(JSON_PARSER ${JSON_PARSER_SOURCES})

# Optional: If you have a main.cpp directly in project root
add_executable(JSON_PARSER src/main.cpp)

# Optional: tests
if(BUILD_TESTS)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    add_executable(JSON_PARSER_TESTS ${TEST_SOURCES})

    add_test(NAME JSON_PARSER_TESTS COMMAND JSON_PARSER_TESTS)
endif()
