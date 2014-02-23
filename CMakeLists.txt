cmake_minimum_required(VERSION 2.8)

project(lapdog)

add_executable(lapdog advsystem.cpp configuration.cpp scanner.cpp devices.cpp device.cpp arp_table_entry.cpp arp.cpp main.cpp)
target_link_libraries(lapdog oping)

install(TARGETS lapdog RUNTIME DESTINATION /usr/sbin/)
install(FILES init/lapdog DESTINATION /etc/init.d/
  PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
