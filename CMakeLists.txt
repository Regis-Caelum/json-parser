cmake_minimum_required(VERSION 3.20)

project(LIBJSONPARSER
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/std:c++latest)
endif()

file(GLOB_RECURSE LIBJSONPARSER_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(JSONPARSER STATIC ${LIBJSONPARSER_SOURCES})


target_include_directories(JSONPARSER
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_features(JSONPARSER PUBLIC cxx_std_23)

option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

    add_executable(JSON_PARSER_TESTS ${TEST_SOURCES})

    target_include_directories(JSON_PARSER_TESTS
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
    )

    file(GLOB_RECURSE SRC_SOURCES "src/*.cpp")
    target_sources(JSON_PARSER_TESTS PRIVATE ${SRC_SOURCES})

    target_link_libraries(JSON_PARSER_TESTS
        PRIVATE
        gtest_main
    )

    add_test(NAME JSON_PARSER_TESTS COMMAND JSON_PARSER_TESTS)
endif()

