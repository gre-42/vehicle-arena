include(../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaPoly2Tri "1" "" "${excluded_files}")

no_warn_conversion(VehicleArenaPoly2Tri)

