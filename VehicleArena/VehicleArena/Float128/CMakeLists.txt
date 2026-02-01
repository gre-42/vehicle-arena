include(../../CMakeCommands.cmake)

set(mlib_excluded_files "")

my_add_library(MlibFloat128 "1" "" "${mlib_excluded_files}")

target_include_directories(MlibFloat128 PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(MlibFloat128 -lquadmath)
