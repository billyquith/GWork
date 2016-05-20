# Find Allegro 5.x

set(ALLEGRO_VERSION "5.2")  # Required version
message("Looking for Allegro ${ALLEGRO_VERSION}")

# Find headers
find_path(ALLEGRO_INCLUDE
    NAMES
        allegro5/allegro5.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
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

# Find components
foreach(ALLEGRO_LIB_EACH ${ALLEGRO_LIB_LIST})    
    find_library(${ALLEGRO_LIB_EACH}_LIB
        NAMES
            "${ALLEGRO_LIB_EACH}.${ALLEGRO_VERSION}"
        PATHS
            /usr/lib
            /usr/local/lib
            /opt/local/lib
            /sw/lib
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


