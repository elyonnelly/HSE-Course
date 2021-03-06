cmake_minimum_required(VERSION 3.16)

include(GoogleTest)

project("runner")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

################ gtest ################
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .

  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )

if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

################  Sanitizers  ################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold -fsanitize=undefined,address -fno-sanitize-recover=all -O2 -Wall -Werror -Wsign-compare")

################  clang-tidy  ################
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=.")

add_executable(runner tests.cpp)

################ clang-format ################
list(APPEND CMAKE_MODULE_PATH $ENV{CLANG_FORMAT_SUBMODULE}/cmake)
include(ClangFormat)
target_clangformat_setup(runner)

add_subdirectory(src/allocator)
add_subdirectory(src/list)

target_link_libraries(runner LINK_PUBLIC list allocator gtest_main)

add_test(NAME runner_test COMMAND runner)