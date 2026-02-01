include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaMemory "1" "" "${excluded_files}")

target_include_directories(VehicleArenaMemory PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaMemory VehicleArenaOs VehicleArenaMisc)
