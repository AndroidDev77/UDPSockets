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

set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL REQUIRED)

if (OPENSSL_FOUND)
    
  # Add the include directories for compiling
  include_directories(${OPENSSL_INCLUDE_DIR})
  # Add the static lib for linking
  #link_libraries(OPENSSL_CRYOPENSSL_LIBRARIES)
  message(STATUS "Found OpenSSL ${OPENSSL_VERSION}")
else()
  message(STATUS "OpenSSL Not Found")
endif()


include_directories(${PROJECT_SOURCE_DIR})

add_executable (run_tests ../PODLPacket.cpp ../PODLServer.cpp  ../PODLClient.cpp  PODLPacketTest.cpp )
target_link_libraries(run_tests PRIVATE CppUTest CppUTestExt OpenSSL::Crypto pthread)
