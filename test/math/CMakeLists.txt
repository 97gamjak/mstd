if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    project(mstd_tests_math LANGUAGES CXX)
    include(CTest)
    enable_testing()
    set(MSTD_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
else()
    set(MSTD_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")
endif()

if(NOT TARGET mstd)
    add_library(mstd INTERFACE)
    target_include_directories(mstd
        INTERFACE
        "${MSTD_ROOT_DIR}/include"
    )
    target_compile_features(mstd INTERFACE cxx_std_20)
endif()

if(NOT TARGET Catch2::Catch2WithMain)
    add_subdirectory(
        "${MSTD_ROOT_DIR}/external/Catch2"
        "${CMAKE_CURRENT_BINARY_DIR}/external/Catch2"
    )
endif()

list(APPEND CMAKE_MODULE_PATH "${MSTD_ROOT_DIR}/external/Catch2/extras")

if(TARGET mstd_test_support)
    set(MSTD_TEST_LINK_TARGET mstd_test_support)
else()
    add_library(mstd_test_support INTERFACE)
    if(EXISTS "${MSTD_ROOT_DIR}/test/include")
        target_include_directories(mstd_test_support
            INTERFACE
            "${MSTD_ROOT_DIR}/test/include"
        )
    endif()
    target_link_libraries(mstd_test_support
        INTERFACE
        mstd
        Catch2::Catch2WithMain
    )
    target_compile_features(mstd_test_support INTERFACE cxx_std_20)
    set(MSTD_TEST_LINK_TARGET mstd_test_support)
endif()

add_executable(mstd_tests_math
    test_cpow.cpp
)

target_link_libraries(mstd_tests_math
    PRIVATE
    "${MSTD_TEST_LINK_TARGET}"
)

target_compile_features(mstd_tests_math PRIVATE cxx_std_20)

include(Catch)
catch_discover_tests(mstd_tests_math
    TEST_PREFIX "mstd::math::"
    REPORTER compact
)

set_property(GLOBAL APPEND PROPERTY MSTD_TEST_TARGETS mstd_tests_math)
