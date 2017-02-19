# Find Ponder C++ reflection library.

set(FIND_PONDER_PATHS
    ~/Library/Frameworks
    /Library/Frameworks
    /usr/local
    /usr
    /sw
    /opt/local
    /opt/csw
    /opt)

# find header path
find_path(PONDER_INCLUDE_DIR ponder/version.hpp
          PATH_SUFFIXES include
          PATHS ${FIND_PONDER_PATHS})
          
# check the version number
set(PONDER_VERSION_OK TRUE)
if(Ponder_FIND_VERSION AND PONDER_INCLUDE_DIR)
    # extract the major and minor version numbers from ponder/version.hpp
    # we have to handle framework a little bit differently:
    if("${PONDER_INCLUDE_DIR}" MATCHES "ponder.framework")
        set(PONDER_CONFIG_HPP_INPUT "${PONDER_INCLUDE_DIR}/Headers/version.hpp")
    else()
        set(PONDER_CONFIG_HPP_INPUT "${PONDER_INCLUDE_DIR}/ponder/version.hpp")
    endif()
    
    FILE(READ "${PONDER_CONFIG_HPP_INPUT}" PONDER_VERSION_HPP_CONTENTS)
    STRING(REGEX REPLACE ".*#define PONDER_VERSION_STR \"([0-9\\.]+)\".*" "\\1" PONDER_VERSION_STR "${PONDER_VERSION_HPP_CONTENTS}")
    STRING(REGEX REPLACE ".*#define PONDER_VERSION[ \t]+\\((.*)\\).*" "\\1" PONDER_VERSION_CALC_STR "${PONDER_VERSION_HPP_CONTENTS}")

    math(EXPR PONDER_FOUND_CALC "${PONDER_VERSION_CALC_STR}")
    message("Found Ponder version ${PONDER_VERSION_STR}") #" ${PONDER_FOUND_CALC}")

    math(EXPR PONDER_REQUESTED_CALC "${Ponder_FIND_VERSION_MAJOR} * 1000000 + ${Ponder_FIND_VERSION_MINOR} * 1000 + ${Ponder_FIND_VERSION_PATCH}")

    # compare them
    if(PONDER_FOUND_CALC LESS PONDER_REQUESTED_CALC)
        set(PONDER_FOUND FALSE)
        message("Ponder version not recent enough. Requested ${Ponder_FIND_VERSION}, found ${PONDER_VERSION_STR}.")
    else()
        set(PONDER_FOUND TRUE)
    endif()
endif()

find_library(PONDER_LIBRARIES
             NAMES ponder
             PATH_SUFFIXES lib
             PATHS ${FIND_PONDER_PATHS})

