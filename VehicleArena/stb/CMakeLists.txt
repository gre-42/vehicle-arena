include(../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaStb "1" "" "${excluded_files}")

no_warn_conversion(VehicleArenaStb)

target_include_directories(VehicleArenaStb PUBLIC ${VehicleArena_INCLUDE_DIR})
