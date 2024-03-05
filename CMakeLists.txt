cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_policy(SET CMP0144 NEW)

project(rv32i-sv)
find_package(verilator REQUIRED)

include(ExternalProject)
ExternalProject_Add(
	rubbler
	GIT_REPOSITORY https://github.com/fuad1502/rubbler.git
	DOWNLOAD_DIR ${CMAKE_BINARY_DIR}
	SOURCE_DIR ${CMAKE_BINARY_DIR}/rubbler
	BINARY_DIR ${CMAKE_BINARY_DIR}/rubbler
	CONFIGURE_COMMAND ""
	INSTALL_COMMAND ""
	BUILD_COMMAND cargo build --release
)
set(RUBBLER_LIB ${CMAKE_BINARY_DIR}/rubbler/target/release/librubbler.a)
include_directories(${CMAKE_BINARY_DIR}/rubbler/target)

enable_testing()
add_subdirectory(testsuits)
