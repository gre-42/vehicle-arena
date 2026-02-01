include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaStats "1" "" "${excluded_files}")

target_include_directories(VehicleArenaStats PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaStats VehicleArenaOs)
