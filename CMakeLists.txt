cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(mstd
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "mstd - header-only C++ library"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
        message(FATAL_ERROR "Insufficient gcc version (need >= 14.0)")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with GNU.")
endif()

option(MSTD_USE_MP_UNITS "Enable vendored mp-units support" ON)
set(MSTD_MP_UNITS_CONTRACTS "NONE" CACHE STRING "Contract checking mode for vendored mp-units: NONE, GSL-LITE, or MS-GSL")
set_property(CACHE MSTD_MP_UNITS_CONTRACTS PROPERTY STRINGS NONE GSL-LITE MS-GSL)
option(MSTD_BUILD_TESTS "Build mstd test" ON)
option(MSTD_BUILD_DOCS "Build mstd documentation" OFF)

add_library(mstd INTERFACE)
target_include_directories(mstd
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(mstd
    INTERFACE
    cxx_std_23
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(mstd INTERFACE
        -Wall
        -Wextra
        -Wpedantic
        -Werror

        # correctness
        -Wshadow
        -Wconversion
        -Wsign-conversion
        -Wcast-align
        -Wformat=2
        -Wnull-dereference
        -Wdouble-promotion
        -Wimplicit-fallthrough
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wuseless-cast
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wmisleading-indentation

        # lifetime / safety
        -Wuninitialized
        -Wmaybe-uninitialized
        -Winit-self

        -Wno-error=deprecated-declarations
    )
endif()

if(MSTD_USE_MP_UNITS)
    if(CMAKE_VERSION VERSION_LESS "3.25")
        message(FATAL_ERROR "MSTD_USE_MP_UNITS requires CMake 3.25 or newer")
    endif()
    set(MP_UNITS_API_CONTRACTS ${MSTD_MP_UNITS_CONTRACTS} CACHE STRING "Enable contract checking" FORCE)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/mp-units/src/CMakeLists.txt")
        add_subdirectory(
            "${CMAKE_CURRENT_SOURCE_DIR}/external/mp-units/src"
            "${CMAKE_CURRENT_BINARY_DIR}/external/mp-units"
        )
        if(TARGET mp-units)
            add_library(mstd::mp-units ALIAS mp-units)
        elseif(TARGET mp-units::mp-units)
            get_target_property(_mp_units_target mp-units::mp-units ALIAS_TARGET)
            add_library(mstd::mp-units ALIAS ${_mp_units_target})
        else()
            message(FATAL_ERROR "Could not find an mp-units target to alias")
        endif()
    else()
        message(FATAL_ERROR "Vendored mp-units source not found in external/mp-units/src")
    endif()
endif()

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
