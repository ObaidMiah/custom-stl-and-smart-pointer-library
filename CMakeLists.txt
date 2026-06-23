cmake_minimum_required(VERSION 3.16)

# Project definition
project(CustomSTLLibrary
    VERSION 1.0.0
    DESCRIPTION "Custom STL and Smart Pointer Library"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4 /WX)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Od /Zi)
    else()
        add_compile_options(/O2)
    endif()
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    else()
        add_compile_options(-O2 -DNDEBUG)
    endif()
endif()

# Create an interface library for the vector headers
add_library(vector_lib INTERFACE)
target_include_directories(vector_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(vector_lib INTERFACE cxx_std_17)

# Specify header files as sources for better IDE support
target_sources(vector_lib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/vector.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/vector.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/list.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/list.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/unique_ptr.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/unique_ptr.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/shared_ptr.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/shared_ptr.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/map.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/map.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/stack.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/stack.hpp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/queue.h>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/queue.hpp>
)

# Option to build tests
option(BUILD_TESTS "Build test executables" ON)

if(BUILD_TESTS)
    # Check for local Google Test installation first
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/gtest_build/install")
        set(GTest_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/gtest_build/install")
        message(STATUS "Using local Google Test installation at ${GTest_ROOT}")
    endif()
    
    # Find Google Test (required)
    find_package(GTest REQUIRED)
    message(STATUS "Found Google Test, building tests")
    
    # Test executables
    add_executable(test_vector_gtest tests/test_vector_gtest.cpp)
    target_link_libraries(test_vector_gtest
        PRIVATE
        vector_lib
        GTest::gtest_main
    )

    add_executable(test_list_gtest tests/test_list_gtest.cpp)
    target_link_libraries(test_list_gtest
        PRIVATE
        vector_lib
        GTest::gtest_main
    )

    add_executable(test_unique_ptr_gtest tests/test_unique_ptr_gtest.cpp)
    target_link_libraries(test_unique_ptr_gtest
        PRIVATE
        vector_lib
        GTest::gtest_main
    )

    add_executable(test_shared_ptr_gtest tests/test_shared_ptr_gtest.cpp)
    target_link_libraries(test_shared_ptr_gtest
        PRIVATE
        vector_lib
        GTest::gtest_main
    )

    add_executable(test_map_gtest tests/test_map_gtest.cpp)
    target_link_libraries(test_map_gtest
        PRIVATE
        vector_lib
        GTest::gtest_main
    )

    add_executable(test_stack_gtest tests/test_stack_gtest.cpp)
    target_link_libraries(test_stack_gtest
        PRIVATE
        vector_lib
        GTest::gtest_main
    )

    add_executable(test_queue_gtest tests/test_queue_gtest.cpp)
    target_link_libraries(test_queue_gtest
        PRIVATE
        vector_lib
        GTest::gtest_main
    )

    # Benchmark executable
    add_executable(benchmark_vector benchmarks/benchmark_vector.cpp)
    target_link_libraries(benchmark_vector 
        PRIVATE 
        vector_lib
    )
    
    # Enable testing
    enable_testing()
    add_test(NAME VectorTests COMMAND test_vector_gtest)
    add_test(NAME ListTests COMMAND test_list_gtest)
    add_test(NAME UniquePtrTests COMMAND test_unique_ptr_gtest)
    add_test(NAME SharedPtrTests COMMAND test_shared_ptr_gtest)
    add_test(NAME MapTests COMMAND test_map_gtest)
    add_test(NAME StackTests COMMAND test_stack_gtest)
    add_test(NAME QueueTests COMMAND test_queue_gtest)

    # Set test properties
    set_target_properties(
        test_vector_gtest test_list_gtest test_unique_ptr_gtest test_shared_ptr_gtest
        test_map_gtest test_stack_gtest test_queue_gtest benchmark_vector PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
endif()

# Installation rules
include(GNUInstallDirs)

# Install the header files
install(FILES
    include/vector.h include/vector.hpp
    include/list.h include/list.hpp
    include/unique_ptr.h include/unique_ptr.hpp
    include/shared_ptr.h include/shared_ptr.hpp
    include/map.h include/map.hpp
    include/stack.h include/stack.hpp
    include/queue.h include/queue.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/custom_stl
)

# Install the interface library
install(TARGETS vector_lib
    EXPORT CustomSTLTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Create and install the config files
install(EXPORT CustomSTLTargets
    FILE CustomSTLTargets.cmake
    NAMESPACE CustomSTL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CustomSTL
)

# Print configuration summary
message(STATUS "")
message(STATUS "Custom STL Library Configuration Summary:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
if(BUILD_TESTS)
    message(STATUS "  Testing framework: Google Test")
endif()
message(STATUS "")
