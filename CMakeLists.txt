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

add_executable(${PROJECT_NAME} ${CHRIS_COIN_SOURCES})
target_link_libraries(${PROJECT_NAME} uv)
