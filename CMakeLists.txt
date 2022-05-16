cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if (CMAKE_CURRENT_BINARY_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  message(FATAL_ERROR "In source-tree build not support")
endif()

set(PACKAGE_NAME            "netconcepts")

# Defer enabling C and CXX languages.
project(${PACKAGE_NAME} NONE)

## Export build compilation database if possible
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# C11/C++17 compiler flags setup.
# --------------------------
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_language(C)
enable_language(CXX)

add_executable(nc src/nc.cc)
