# CMake build instructions for Gwork

cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0004 OLD)

project(gwork)

set(CMAKE_FIND_FRAMEWORK LAST)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(GWORK_SOURCE_DIR ${PROJECT_SOURCE_DIR})
 
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

add_subdirectory(gwen)


