include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaThreads "1" "" "${excluded_files}")

target_include_directories(VehicleArenaThreads PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaThreads VehicleArenaOs VehicleArenaTime)
