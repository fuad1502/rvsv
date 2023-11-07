cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0144 NEW)

project(rv32i-sv)
find_package(verilator REQUIRED)

enable_testing()
add_subdirectory(testsuits)
