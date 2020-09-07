cmake_minimum_required(VERSION 3.18)

project(ChrisCoin)
set(CMAKE_CXX_STANDARD 14)

set(CHRIS_COIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CHRIS_COIN_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set( CHRIS_COIN_SOURCES
  src/main.cpp
  src/web/web.cpp
  src/core/blockchain.cpp
  )

find_package(Boost 1.47 COMPONENTS thread system REQUIRED)

include_directories(include ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG 5e87cb5f45ac99858f0286dc1c35a6cd27c3bcb9) # the commit hash for 1.5.1
FetchContent_MakeAvailable(cpr)

add_executable(${PROJECT_NAME} ${CHRIS_COIN_SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES} PRIVATE cpr::cpr)
