# CMake build instructions for Gwork

cmake_minimum_required(VERSION 3.0)
cmake_policy(SET CMP0004 OLD)

project(gwork)

# configure cmake
set(CMAKE_FIND_FRAMEWORK LAST)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
             
# output directory
set(CMAKE_BINARY_DIR "${CMAKE_HOME_DIRECTORY}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # exe
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # .a
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # .dylib
make_directory(${CMAKE_BINARY_DIR})
make_directory(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
make_directory(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

file(COPY ${CMAKE_HOME_DIRECTORY}/gwen/bin/DefaultSkin.png
          ${CMAKE_HOME_DIRECTORY}/gwen/bin/gwen.png
          ${CMAKE_HOME_DIRECTORY}/gwen/bin/OpenSans.ttf
          ${CMAKE_HOME_DIRECTORY}/gwen/bin/test16.png
     DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
 
# configure projects
include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

# add our projects
add_subdirectory(gwen)


