include(../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaStb "1" "" "${vehicle_arena_excluded_files}")

no_warn_conversion(VehicleArenaStb)

include_directories(${VehicleArena_INCLUDE_DIR})
