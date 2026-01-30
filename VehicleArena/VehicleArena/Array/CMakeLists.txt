include(../../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaArray "1" "" "${vehicle_arena_excluded_files}")

target_include_directories(VehicleArenaArray PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaArray VehicleArenaIo)
