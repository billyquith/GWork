# Find Allegro 5.x

set(ALLEGRO_VERSION "5.2")  # Required version
message("Looking for Allegro ${ALLEGRO_VERSION}")

# Allegro binaries can get retrieved from NuGet for Windows.
#   - MSVC: Tools > NuGet Package Manager > Package Manager Console
#   - PM> Install-Package Allegro

# Find headers
find_path(ALLEGRO_INCLUDE
    NAMES
        allegro5/allegro5.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
        ${CMAKE_BINARY_DIR}/packages/Allegro.5.*/build/native/include  # NuGet
)

# Components to find
set(ALLEGRO_LIB_LIST
    allegro
    allegro_dialog
    allegro_font
    allegro_image
    allegro_main
    allegro_primitives
    allegro_ttf
)

mark_as_advanced(ALLEGRO_INCLUDE ALLEGRO_LIB_LIST)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(AL_LIB_TYPE v140/x64)
else()
    set(AL_LIB_TYPE v140/win32)
endif()

# Find components
foreach(ALLEGRO_LIB_EACH ${ALLEGRO_LIB_LIST})
    find_library(${ALLEGRO_LIB_EACH}_LIB
        NAMES
            "${ALLEGRO_LIB_EACH}.${ALLEGRO_VERSION}"
            "${ALLEGRO_LIB_EACH}"
        PATHS
            /usr/lib
            /usr/local/lib
            /opt/local/lib
            /sw/lib
            ${CMAKE_BINARY_DIR}/packages/Allegro.5.*/build/native/${AL_LIB_TYPE}/lib  # NuGet
    )
    message("Component: ${ALLEGRO_LIB_EACH} -> ${${ALLEGRO_LIB_EACH}_LIB}")
    list(APPEND ALLEGRO_LIBRARIES ${${ALLEGRO_LIB_EACH}_LIB})
    mark_as_advanced(${ALLEGRO_LIB_EACH}_LIB)
endforeach()

# Return values
set(ALLEGRO5_INCLUDE_DIRS ${ALLEGRO_INCLUDE})
set(ALLEGRO5_LIBRARIES ${ALLEGRO_LIBRARIES})

# Check results
if (ALLEGRO5_INCLUDE_DIRS AND ALLEGRO5_LIBRARIES)
    set(ALLEGRO5_FOUND TRUE)
endif (ALLEGRO5_INCLUDE_DIRS AND ALLEGRO5_LIBRARIES)

if (ALLEGRO5_FOUND)
    if (NOT Allegro_FIND_QUIETLY)
        message(STATUS "Found Allegro: ${ALLEGRO5_LIBRARIES}")
    endif (NOT Allegro_FIND_QUIETLY)
else (ALLEGRO5_FOUND)
    if (Allegro_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find Allegro")
    endif (Allegro_FIND_REQUIRED)
endif (ALLEGRO5_FOUND)
