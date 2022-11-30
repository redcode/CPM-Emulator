cmake_minimum_required(VERSION 3.3)

if(	CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR AND
	NOT CMAKE_BUILD_TYPE AND
	NOT CMAKE_CONFIGURATION_TYPES
)
	set(CMAKE_BUILD_TYPE Release)
endif()

project(
	CPM-Emulator
	LANGUAGES C CXX
	DESCRIPTION "Yet another Z80 Computer with CP/M capabilities (no BIOS, emulated BDOS, executed CP/M)")

set(Z80_SHARED_LIBS NO CACHE BOOL "")
set(Z80_WITH_EXECUTE YES CACHE BOOL "")
set(Z80_WITH_Q YES CACHE BOOL "")
set(Z80_WITH_ZILOG_NMOS_LD_A_IR_BUG YES CACHE BOOL "")

add_subdirectory(external/Z80)
add_executable(cpm "${CMAKE_CURRENT_SOURCE_DIR}/sources/main.cpp")
target_link_libraries(cpm PRIVATE Z80)
install(TARGETS cpm RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
