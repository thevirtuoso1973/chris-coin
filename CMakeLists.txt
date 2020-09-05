cmake_minimum_required(VERSION 3.18)

project(ChrisCoin)

set(CHRIS_COIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CHRIS_COIN_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set( CHRIS_COIN_SOURCES
  src/main.cpp
  src/web/web.cpp
  )

find_package(Boost 1.47 COMPONENTS thread system REQUIRED)

include_directories(include ${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${CHRIS_COIN_SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
