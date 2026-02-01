include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaAudio "1" "" "${excluded_files}")

target_include_directories(VehicleArenaAudio PUBLIC ${VehicleArena_INCLUDE_DIR} ${ALUT_INCLUDE_DIRS})

target_link_libraries(VehicleArenaAudio VehicleArenaMisc VehicleArenaThreads VehicleArenaMemory VehicleArenaTime VehicleArenaSceneGraph ${ALUT_LIBRARIES})
