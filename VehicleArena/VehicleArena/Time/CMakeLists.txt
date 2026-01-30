include(../../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaTime "1" "" "${vehicle_arena_excluded_files}")

target_link_libraries(VehicleArenaTime VehicleArenaOs)

include_directories(${VehicleArena_INCLUDE_DIR})
