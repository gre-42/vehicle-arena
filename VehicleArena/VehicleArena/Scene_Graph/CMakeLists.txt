include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaSceneGraph "1" "" "${excluded_files}")

target_include_directories(VehicleArenaSceneGraph PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaSceneGraph VehicleArenaGeometry)
