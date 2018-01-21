# CMake build instructions for Gwork

# configure cmake
cmake_minimum_required(VERSION 3.1)

# we require C++11 - this set appropriate flags for compilers, which may not be portable
set(CMAKE_CXX_STANDARD 14)

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

# write the version info
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gwk_config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/source/platform/include/Gwork/Version.h
    @ONLY)

# add our projects
add_subdirectory(source)
add_subdirectory(deps)
add_subdirectory(docs)
