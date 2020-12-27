# Install script for directory: /home/chris/binaryParser/build/_deps/cpputest-src/src/CppUTest

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/chris/binaryParser/build/_deps/cpputest-build/src/CppUTest/libCppUTest.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CppUTest" TYPE FILE FILES
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/CommandLineArguments.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/PlatformSpecificFunctions.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestMemoryAllocator.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/CommandLineTestRunner.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/PlatformSpecificFunctions_c.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestOutput.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/CppUTestConfig.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/SimpleString.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/SimpleStringInternalCache.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestPlugin.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/JUnitTestOutput.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TeamCityTestOutput.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/StandardCLibrary.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestRegistry.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/MemoryLeakDetector.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestFailure.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestResult.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/MemoryLeakDetectorMallocMacros.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestFilter.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestTestingFixture.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/MemoryLeakDetectorNewMacros.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestHarness.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/Utest.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/MemoryLeakWarningPlugin.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/TestHarness_c.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/UtestMacros.h"
    "/home/chris/binaryParser/build/_deps/cpputest-src/include/CppUTest/SimpleMutex.h"
    )
endif()

