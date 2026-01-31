include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaGeometry "1" "" "${excluded_files}")

target_include_directories(VehicleArenaGeometry PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaGeometry VehicleArenaMath)
