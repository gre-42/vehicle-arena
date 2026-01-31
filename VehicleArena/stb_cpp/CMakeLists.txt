include(../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaStbCpp "1" "" "${excluded_files}")
target_include_directories(VehicleArenaStbCpp PUBLIC ${VehicleArena_INCLUDE_DIR})
target_link_libraries(VehicleArenaStbCpp VehicleArenaStb VehicleArenaMisc VehicleArenaArray)
