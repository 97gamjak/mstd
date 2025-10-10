cmake_minimum_required(VERSION 3.20)

project(mstd
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "mstd - header-only C++ library"
)

option(MSTD_BUILD_TESTS "Build mstd test" ON)

add_library(mstd INTERFACE)
target_include_directories(mstd
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(mstd
    INTERFACE
    cxx_std_20
)

if(MSTD_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/external/Catch2
    )
    file(
        GLOB_RECURSE
        MSTD_TEST_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp
    )

    add_executable(
        mstd_tests
        ${MSTD_TEST_SOURCES}
    )
    target_link_libraries(
        mstd_tests
        PRIVATE
        mstd
        Catch2::Catch2WithMain
    )
    target_include_directories(
        mstd_tests
        SYSTEM PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/external/Catch2/src
    )
    target_compile_features(mstd_tests
        PRIVATE
        cxx_std_20
    )

    include(Catch)
    catch_discover_tests(mstd_tests
        TEST_PREFIX "mstd::"
        REPORTER compact
    )
else()
    message(STATUS "Tests are disabled. Set MSTD_BUILD_TESTS to ON to enable them.")
endif()
