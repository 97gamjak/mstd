cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mstd
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "mstd - header-only C++ library"
)

option(MSTD_BUILD_TESTS "Build mstd test" ON)
option(MSTD_BUILD_DOCS "Build mstd documentation" OFF)

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
    add_subdirectory(test)
else()
    message(STATUS "Tests are disabled. Set MSTD_BUILD_TESTS to ON to enable them.")
endif()

if(MSTD_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    message(STATUS "Doxygen found. Use 'make docs' to build documentation.")
else()
    message(STATUS "Documentation is disabled. Set MSTD_BUILD_DOCS to ON to enable it.")
endif()
