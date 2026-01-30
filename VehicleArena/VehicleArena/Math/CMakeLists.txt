include(../../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaMath "1" "" "${vehicle_arena_excluded_files}")

target_include_directories(VehicleArenaMath PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaMath VehicleArenaOs VehicleArenaArray VehicleArenaJson)
