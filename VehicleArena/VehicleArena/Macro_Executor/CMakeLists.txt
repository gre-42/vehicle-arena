include(../../CMakeCommands.cmake)

set(mlib_excluded_files "")

my_add_library(MlibMacroExecutor "1" "" "${mlib_excluded_files}")

target_include_directories(MlibMacroExecutor PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(MlibMacroExecutor MlibJson MlibRegex MlibThreads MlibMemory)
