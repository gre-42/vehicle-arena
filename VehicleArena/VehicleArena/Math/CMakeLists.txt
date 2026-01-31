include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaMath "1" "" "${excluded_files}")

target_include_directories(VehicleArenaMath PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaMath VehicleArenaOs VehicleArenaArray VehicleArenaJson)
