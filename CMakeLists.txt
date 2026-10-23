cmake_minimum_required(VERSION 3.25)

project(mstd
    VERSION 0.1.0
    LANGUAGES CXX
    DESCRIPTION "mstd - header-only C++ library"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(mstd_IS_TOP_LEVEL)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 14.0)
        message(FATAL_ERROR "Insufficient gcc version (need >= 14.0)")
    endif()
elseif(mstd_IS_TOP_LEVEL)
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with GNU.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(_mstd_debug_default ON)
else()
    set(_mstd_debug_default OFF)
endif()

# Tests/docs are only built by default when mstd is the top-level project
option(MSTD_DEBUG "Enable mstd debug checks" ${_mstd_debug_default})
option(MSTD_USE_MP_UNITS "Enable vendored mp-units support" ON)
set(MSTD_MP_UNITS_CONTRACTS "NONE" CACHE STRING "Contract checking mode for vendored mp-units: NONE, GSL-LITE, or MS-GSL")
set_property(CACHE MSTD_MP_UNITS_CONTRACTS PROPERTY STRINGS NONE GSL-LITE MS-GSL)
option(MSTD_BUILD_TESTS "Build mstd test" ${mstd_IS_TOP_LEVEL})
option(MSTD_BUILD_DOCS "Build mstd documentation" OFF)

# ---------------------------------------------------------------------------
# mstd target
# ---------------------------------------------------------------------------
add_library(mstd INTERFACE)
add_library(mstd::mstd ALIAS mstd)

# As a dependency of another project, mark the headers SYSTEM so their
# warnings don't show up in the consumer's build.
if(mstd_IS_TOP_LEVEL)
    set(_mstd_include_scope "")
else()
    set(_mstd_include_scope SYSTEM)
endif()

target_include_directories(mstd ${_mstd_include_scope}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(mstd
    INTERFACE
    cxx_std_23
)

if(MSTD_DEBUG)
    target_compile_definitions(mstd INTERFACE MSTD_DEBUG)
endif()

# Strict warnings only for mstd's own build (tests). INTERFACE options would
# otherwise leak -Werror into every consumer such as PQ.
if(mstd_IS_TOP_LEVEL AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
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

# ---------------------------------------------------------------------------
# mp-units (vendored submodule)
# ---------------------------------------------------------------------------
if(MSTD_USE_MP_UNITS)
    set(_mp_units_dir "${CMAKE_CURRENT_SOURCE_DIR}/external/mp-units")

    # Fetch the submodule on demand
    if(NOT EXISTS "${_mp_units_dir}/src/CMakeLists.txt")
        find_package(Git QUIET)
        if(GIT_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
            message(STATUS "Initializing mp-units submodule...")
            execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- external/mp-units
                WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                RESULT_VARIABLE _git_result
                ERROR_VARIABLE  _git_error
            )
            if(NOT _git_result EQUAL 0)
                message(FATAL_ERROR "git submodule update failed (${_git_result}):\n${_git_error}")
            endif()
        endif()
    endif()

    if(NOT EXISTS "${_mp_units_dir}/src/CMakeLists.txt")
        message(FATAL_ERROR "Vendored mp-units source not found in external/mp-units/src "
                            "(git unavailable or not a git checkout?)")
    endif()

    set(MP_UNITS_API_CONTRACTS ${MSTD_MP_UNITS_CONTRACTS} CACHE STRING "Enable contract checking" FORCE)

    add_subdirectory("${_mp_units_dir}/src" "${CMAKE_CURRENT_BINARY_DIR}/external/mp-units")

    if(TARGET mp-units)
        add_library(mstd::mp-units ALIAS mp-units)
    elseif(TARGET mp-units::mp-units)
        get_target_property(_mp_units_target mp-units::mp-units ALIASED_TARGET)
        if(NOT _mp_units_target)
            set(_mp_units_target mp-units::mp-units)
        endif()
        add_library(mstd::mp-units ALIAS ${_mp_units_target})
    else()
        message(FATAL_ERROR "Could not find an mp-units target to alias")
    endif()

    target_link_libraries(mstd INTERFACE mstd::mp-units)
    target_compile_definitions(mstd INTERFACE MSTD_USE_MP_UNITS)
endif()

# ---------------------------------------------------------------------------
# Tests / docs
# ---------------------------------------------------------------------------
if(MSTD_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
else()
    message(STATUS "mstd tests are disabled. Set MSTD_BUILD_TESTS to ON to enable them.")
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
    message(STATUS "mstd documentation is disabled. Set MSTD_BUILD_DOCS to ON to enable it.")
endif()
