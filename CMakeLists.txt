cmake_minimum_required(VERSION 3.3)

if(	CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR AND
	NOT CMAKE_BUILD_TYPE				   AND
	NOT CMAKE_CONFIGURATION_TYPES
)
	set(CMAKE_BUILD_TYPE Release)
endif()

project(
	CPM-Emulator
	LANGUAGES C CXX
	DESCRIPTION "Yet another Z80 Computer with CP/M capabilities (no BIOS, emulated BDOS, executed CP/M)")

find_package(Z80 REQUIRED)

add_executable(
	cpm
	"${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp")

target_link_libraries(cpm PRIVATE Z80)
