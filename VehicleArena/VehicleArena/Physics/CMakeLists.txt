include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaPhysics "1" "" "${excluded_files}")
# https://issuetracker.google.com/issues/214811726
if (NOT ANDROID)
    target_precompile_headers(VehicleArenaPhysics PRIVATE ../Misc/Precompiled_Header.hpp)
endif()

target_include_directories(VehicleArenaPhysics PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaPhysics VehicleArenaGeometry VehicleArenaSceneGraph VehicleArenaAudio VehicleArenaThreads VehicleArenaTime)
