cmake_minimum_required(VERSION 2.8)

project(lapdog)

add_executable(lapdog device.cpp arp_table_entry.cpp arp.cpp main.cpp)
target_link_libraries(lapdog oping)

install(TARGETS lapdog RUNTIME DESTINATION bin)
