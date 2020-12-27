cmake_minimum_required(VERSION 3.0.0)
project(PODLServer VERSION 0.1.0)

set(SOURCE_FILES PODLPacket.cpp PODLServer.cpp PODLClient.cpp main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

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


add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto pthread)


add_subdirectory (Test)

