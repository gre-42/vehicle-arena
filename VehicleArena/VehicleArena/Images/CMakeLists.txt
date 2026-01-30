include(../../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaImages "1" "" "${vehicle_arena_excluded_files}")
# https://issuetracker.google.com/issues/214811726
if (NOT ANDROID)
    target_precompile_headers(VehicleArenaImages PRIVATE ../Misc/Precompiled_Header.hpp)
endif()

include_directories(${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaImages VehicleArenaOs VehicleArenaMath VehicleArenaStbCpp)
