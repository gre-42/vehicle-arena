include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaMisc "1" "" "${excluded_files}")

target_include_directories(VehicleArenaMisc PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaMisc VehicleArenaStrings)
