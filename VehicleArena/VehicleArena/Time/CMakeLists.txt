include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaTime "1" "" "${excluded_files}")

target_link_libraries(VehicleArenaTime VehicleArenaOs)

include_directories(${VehicleArena_INCLUDE_DIR})
