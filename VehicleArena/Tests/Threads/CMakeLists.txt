include(../../CMakeCommands.cmake)

my_add_executable(threading_test "1")

include_directories(VehicleArena ${VehicleArena_INCLUDE_DIR})

add_test(NAME ThreadingTest COMMAND $<TARGET_FILE:threading_test>)
