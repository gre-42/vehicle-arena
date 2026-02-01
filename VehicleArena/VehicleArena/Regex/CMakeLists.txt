include(../../CMakeCommands.cmake)

set(excluded_files "")

my_add_library(VehicleArenaRegex "1" "" "${excluded_files}")

target_include_directories(VehicleArenaRegex PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(VehicleArenaRegex VehicleArenaThreads)
if (MSVC)
    target_link_libraries(VehicleArenaRegex Boost::boost)
endif()
