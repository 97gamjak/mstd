if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    cmake_minimum_required(VERSION 3.20)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    project(mstd_tests LANGUAGES CXX)
    include(CTest)
    enable_testing()
endif()

set(MSTD_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/..")

if(NOT TARGET mstd)
    add_library(mstd INTERFACE)
    target_include_directories(mstd
        INTERFACE
        "${MSTD_ROOT_DIR}/include"
    )
    target_compile_features(mstd INTERFACE cxx_std_20)
endif()

if(NOT TARGET Catch2::Catch2WithMain)
    set(MSTD_CATCH2_SOURCE_DIR "${MSTD_ROOT_DIR}/external/Catch2")
    if(EXISTS "${MSTD_CATCH2_SOURCE_DIR}/CMakeLists.txt")
        add_subdirectory(
            "${MSTD_CATCH2_SOURCE_DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}/external/Catch2"
        )
    else()
        find_package(Git QUIET)
        if(GIT_FOUND)
            message(STATUS "Catch2 submodule missing. Attempting initialization via git...")
            execute_process(
                COMMAND "${GIT_EXECUTABLE}" submodule update --init --recursive -- external/Catch2
                WORKING_DIRECTORY "${MSTD_ROOT_DIR}"
                RESULT_VARIABLE MSTD_CATCH2_SUBMODULE_RESULT
                ERROR_VARIABLE MSTD_CATCH2_SUBMODULE_ERROR
            )
        else()
            set(MSTD_CATCH2_SUBMODULE_RESULT 1)
            set(MSTD_CATCH2_SUBMODULE_ERROR "Git executable was not found.")
        endif()

        if(EXISTS "${MSTD_CATCH2_SOURCE_DIR}/CMakeLists.txt")
            add_subdirectory(
                "${MSTD_CATCH2_SOURCE_DIR}"
                "${CMAKE_CURRENT_BINARY_DIR}/external/Catch2"
            )
        else()
            message(FATAL_ERROR
                "Catch2 submodule is missing at '${MSTD_CATCH2_SOURCE_DIR}'.\n"
                "Automatic submodule initialization failed (code: ${MSTD_CATCH2_SUBMODULE_RESULT}).\n"
                "Details: ${MSTD_CATCH2_SUBMODULE_ERROR}\n"
                "Please run manually and re-run CMake:\n"
                "  git submodule update --init --recursive"
            )
        endif()
    endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${MSTD_CATCH2_SOURCE_DIR}/extras")

if(NOT TARGET mstd_test_support)
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
endif()

file(GLOB MSTD_TEST_CMAKES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*/CMakeLists.txt")

set(MSTD_TEST_DIRECTORIES "")
foreach(test_cmake IN LISTS MSTD_TEST_CMAKES)
    get_filename_component(test_dir "${test_cmake}" DIRECTORY)
    if(test_dir)
        list(APPEND MSTD_TEST_DIRECTORIES "${test_dir}")
    endif()
endforeach()
list(REMOVE_DUPLICATES MSTD_TEST_DIRECTORIES)
list(SORT MSTD_TEST_DIRECTORIES)

if(NOT TARGET mstd_tests)
    add_custom_target(mstd_tests)
endif()

foreach(test_dir IN LISTS MSTD_TEST_DIRECTORIES)
    add_subdirectory("${test_dir}")
endforeach()

get_property(MSTD_ALL_TEST_TARGETS GLOBAL PROPERTY MSTD_TEST_TARGETS)
if(MSTD_ALL_TEST_TARGETS)
    foreach(test_target IN LISTS MSTD_ALL_TEST_TARGETS)
        add_dependencies(mstd_tests "${test_target}")
    endforeach()
endif()
