include(../../CMakeCommands.cmake)

set(mlib_excluded_files "")

my_add_library(MlibRemote "1" "" "${mlib_excluded_files}")
# https://issuetracker.google.com/issues/214811726
if (NOT ANDROID)
    target_precompile_headers(MlibRemote PRIVATE ../Precompiled_Header.hpp)
    target_include_directories(MlibRemote PRIVATE ${MlibEve_INCLUDE_DIR})
endif()

target_include_directories(MlibRemote PUBLIC ${VehicleArena_INCLUDE_DIR})

target_link_libraries(MlibRemote Mlib MlibOs MlibIo MlibMemory MlibJson Boost::boost)

if (MINGW)
    target_link_libraries(MlibRemote -lws2_32)
endif()
