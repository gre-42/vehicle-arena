include(../../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaThreads "1" "" "${vehicle_arena_excluded_files}")

include_directories(${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaThreads VehicleArenaOs VehicleArenaTime)
