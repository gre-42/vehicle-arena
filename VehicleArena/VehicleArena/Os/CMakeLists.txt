include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaOs "1" "" "${excluded_files}")

target_include_directories(VehicleArenaOs PUBLIC ${VehicleArena_INCLUDE_DIR})

if (ANDROID)
    target_link_libraries(VehicleArenaOs NdkHelper)
endif()

target_link_libraries(VehicleArenaOs VehicleArenaStrings)
