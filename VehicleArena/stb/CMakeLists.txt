include(../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaStb "1" "" "${excluded_files}")

no_warn_conversion(VehicleArenaStb)

include_directories(${VehicleArena_INCLUDE_DIR})
