include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaArray "1" "" "${excluded_files}")

target_include_directories(VehicleArenaArray PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaArray VehicleArenaIo)
