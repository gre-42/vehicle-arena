include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaStrings "1" "" "${excluded_files}")

target_include_directories(VehicleArenaStrings PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaStrings Boost::boost)
