cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0144 NEW)

project(rv32i-sv)
find_package(verilator REQUIRED)

include(ExternalProject)
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/rust)
ExternalProject_Add(
  rubbler
  DOWNLOAD_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND cargo build COMMAND cargo build --release
  BINARY_DIR ${CMAKE_SOURCE_DIR}/rubbler
  INSTALL_COMMAND ""
  LOG_BUILD ON
)
include_directories(${CMAKE_SOURCE_DIR}/rubbler)
set(RUBBLER_LIB_DEBUG ${CMAKE_SOURCE_DIR}/rubbler/target/debug/librubbler.a)
set(RUBBLER_LIB_RELEASE ${CMAKE_SOURCE_DIR}/rubbler/target/release/librubbler.a)

enable_testing()
add_subdirectory(testsuits)
