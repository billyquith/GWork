# CMake build instructions for Gwork

# configure cmake
cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0004 OLD)

# we require C++11 - this set appropriate flags for compilers, which may not be portable
set(CMAKE_CXX_STANDARD 11)

project(gwork)

set(GWK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GWK_HEADER_FOLDER "Headers")
set(GWK_SOURCE_FOLDER "Source")

# some build options
set(CMAKE_FIND_FRAMEWORK LAST)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${GWK_SOURCE_DIR}/cmake/Modules/")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
             
# output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GWK_SOURCE_DIR}/bin) # exe
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GWK_SOURCE_DIR}/bin) # .dylib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GWK_SOURCE_DIR}/lib) # .a
make_directory(${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
make_directory(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

# configure projects
include(${GWK_SOURCE_DIR}/cmake/Config.cmake)

# add our projects
add_subdirectory(source)
add_subdirectory(docs)


