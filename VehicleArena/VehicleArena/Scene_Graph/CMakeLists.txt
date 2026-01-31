include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaSceneGraph "1" "" "${excluded_files}")

include_directories(${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaSceneGraph VehicleArenaGeometry)
