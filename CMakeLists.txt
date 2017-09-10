# lapdog - take actions on devices (dis)appearance on the LAN
# Copyright (C) 2014-2017  Salvo Tomaselli <tiposchi@tiscali.it>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.


cmake_minimum_required(VERSION 2.8)

project(lapdog)

set (LAPDOG_VERSION 1.0)
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")


add_executable(lapdog hwaddr.cpp advsystem.cpp configuration.cpp split.cpp scanner.cpp devices.cpp device.cpp arp_table_entry.cpp arp.cpp main.cpp)
target_link_libraries(lapdog oping)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

install(TARGETS lapdog RUNTIME DESTINATION /usr/sbin/)
install(PROGRAMS init/lapdog DESTINATION /etc/init.d/)
install(FILES doc/lapdog.conf DESTINATION /etc/ COMPONENT config)
install(FILES doc/device.conf DESTINATION /etc/lapdog/ COMPONENT config)
install(FILES init/lapdog.service DESTINATION /lib/systemd/system/ COMPONENT config)


