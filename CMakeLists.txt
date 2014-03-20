cmake_minimum_required(VERSION 2.8)

project(lapdog)

add_executable(lapdog advsystem.cpp configuration.cpp split.cpp scanner.cpp devices.cpp device.cpp arp_table_entry.cpp arp.cpp main.cpp)
target_link_libraries(lapdog oping)

install(TARGETS lapdog RUNTIME DESTINATION /usr/sbin/)
install(PROGRAMS init/lapdog DESTINATION /etc/init.d/)
