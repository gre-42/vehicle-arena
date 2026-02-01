include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaImages "1" "" "${excluded_files}")
# https://issuetracker.google.com/issues/214811726
if (NOT ANDROID)
    target_precompile_headers(VehicleArenaImages PRIVATE ../Misc/Precompiled_Header.hpp)
endif()

target_include_directories(VehicleArenaImages PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaImages VehicleArenaOs VehicleArenaMath VehicleArenaStbCpp)
