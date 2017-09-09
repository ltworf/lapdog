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


