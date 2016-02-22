cmake_minimum_required(VERSION 3.0)

project(gwork)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(GWORK_SOURCE_DIR ${PROJECT_SOURCE_DIR})

# set(CMAKE_BINARY_DIR "${GWORK_SOURCE_DIR}/build")

include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

add_subdirectory(gwen)
# add_subdirectory(gwork_test)
# add_subdirectory(doc)

# include_directories("${CMAKE_CURRENT_SOURCE_DIR}/gwen/include/" ${SDL2_INCLUDE_DIR})

add_definitions(-DGWEN_PLATFORM_WINDOWS)

