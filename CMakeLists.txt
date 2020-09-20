cmake_minimum_required(VERSION 3.7...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(
  ChrisCoin
  VERSION 0.1
  DESCRIPTION "Bitcoin client"
  LANGUAGES CXX
  )
set(CMAKE_CXX_STANDARD 17)

set(CHRIS_COIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CHRIS_COIN_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set( CHRIS_COIN_SOURCES
  src/main.cpp
  src/web/messages.cpp
  src/web/node.cpp
  src/web/peers.cpp
  src/core/blockchain.cpp
  )

include_directories(
  extern/uvw/src
  )

add_subdirectory(${PROJECT_SOURCE_DIR}/extern/googletest
                 extern/googletest)

add_executable(${PROJECT_NAME} ${CHRIS_COIN_SOURCES})

target_link_libraries(${PROJECT_NAME} uv)
target_link_libraries(${PROJECT_NAME} ssl)
target_link_libraries(${PROJECT_NAME} crypto)
#target_link_libraries(${PROJECT_NAME} gtest_main)

# tests:
option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
endif()

macro(package_add_test TESTNAME) # from https://cliutils.gitlab.io/modern-cmake/chapters/testing/googletest.html
    # create an exectuable in which the tests will be stored
    add_executable(${TESTNAME} ${ARGN})
    # link the Google test infrastructure, mocking library, and a default main fuction to
    # the test executable.  Remove g_test_main if writing your own main function.
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    # gtest_discover_tests replaces gtest_add_tests,
    # see https://cmake.org/cmake/help/v3.10/module/GoogleTest.html for more options to pass to it
    gtest_discover_tests(${TESTNAME}
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

#package_add_test(test1 test1.cpp)
