include(../../CMakeCommands.cmake)

set(vehicle_arena_excluded_files "")

my_add_library(VehicleArenaOs "1" "" "${vehicle_arena_excluded_files}")

include_directories(${VehicleArena_INCLUDE_DIR})

if (ANDROID)
    target_link_libraries(VehicleArenaOs NdkHelper)
endif()
