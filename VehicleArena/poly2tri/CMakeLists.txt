include(../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaPoly2Tri "1" "" "${vehicle_arena_excluded_files}")

no_warn_conversion(VehicleArenaPoly2Tri)

