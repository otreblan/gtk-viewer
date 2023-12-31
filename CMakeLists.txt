# A C++ template
# Copyright © 2019-2020 otreblan
#
# hello is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# hello is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with hello.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.13.0)

project(gtk-viewer
	VERSION 4.0
	DESCRIPTION "A live image viewer"
	HOMEPAGE_URL "https://github.com/otreblan/gtk-viewer"
)

# Without this, paths are not relative in the sources list
cmake_policy(SET CMP0076 NEW)

include(GNUInstallDirs)
include(FindPkgConfig)
include(CPack)

# The program itself
add_executable(${PROJECT_NAME})

# C++ version
set_target_properties(${PROJECT_NAME}
	PROPERTIES
		CXX_STANDARD 17
)

# Headers
target_include_directories(${PROJECT_NAME}
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(src) # Sources list
add_subdirectory(doc) # Documentation
add_subdirectory(pkg) # Packaging

# Shared libraries
pkg_check_modules(shared
	REQUIRED IMPORTED_TARGET
		gtkmm-4.0
)

## Header only libraries
#pkg_check_modules(header_only
#	REQUIRED IMPORTED_TARGET
#		libfoo3
#		libfoo4
#)

# Linking
target_link_libraries(${PROJECT_NAME}
	PRIVATE
		PkgConfig::shared

#	INTERFACE
#		PkgConfig::header_only
)

# Default flags
if(UNIX)
	if(NOT (DEFINED ENV{CFLAGS} OR CMAKE_C_FLAGS))
		set(CMAKE_C_FLAGS "-Wall -Wextra -g")
	endif()
	if(NOT (DEFINED ENV{CXXFLAGS} OR CMAKE_CXX_FLAGS))
		set(CMAKE_CXX_FLAGS "-Wall -Wextra -g")
	endif()
endif()

# Install target
install(TARGETS ${PROJECT_NAME}
	DESTINATION ${CMAKE_INSTALL_BINDIR}
)
