# CppUTest
project (TEST)
include(FetchContent)
FetchContent_Declare(
    CppUTest
    GIT_REPOSITORY https://github.com/cpputest/cpputest.git
    GIT_TAG        latest-passing-build # or use release tag, eg. v3.8
)
# Set this to ON if you want to have the CppUTests in your project as well.
set(TESTS OFF CACHE BOOL "Switch off CppUTest Test build")
FetchContent_MakeAvailable(CppUTest)

include_directories(${PROJECT_SOURCE_DIR})

add_executable (run_tests ../PODLPacket.cpp PODLPacketTest.cpp)
target_link_libraries(run_tests PRIVATE CppUTest CppUTestExt)