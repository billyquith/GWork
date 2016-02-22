# Find Allegro 5.0

# Find headers
find_path(ALLEG_INCLUDE
    NAMES
        allegro5/allegro5.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /sw/include
    # PATH_SUFFIXES
    #     allegro5
)
# message("ALLEG_INCLUDE ${ALLEG_INCLUDE}")

# Components to find
set(ALLEG_LIB_LIST
    allegro
    allegro_dialog
    allegro_font
    allegro_image
    allegro_main
    allegro_primitives
    allegro_ttf
)

# Find components
foreach(ALLEG_LIB_EACH ${ALLEG_LIB_LIST})    
    find_library(ALLEG_LIBRARY
        NAMES
            "${ALLEG_LIB_EACH}.5.0"
        PATHS
            /usr/lib
            /usr/local/lib
            /opt/local/lib
            /sw/lib
    )
    message("Component: ${ALLEG_LIB_EACH} -> ${ALLEG_LIBRARY}")
    list(APPEND ALLEG_LIBRARIES ${ALLEG_LIBRARY})
    unset(ALLEG_LIBRARY CACHE) # need to reset or find_library fails
endforeach()
# message("ALLEG_LIBRARIES ${ALLEG_LIBRARIES}")

# Return values
set(ALLEGRO5_INCLUDE_DIRS ${ALLEG_INCLUDE})
set(ALLEGRO5_LIBRARIES ${ALLEG_LIBRARIES})

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

# show the ALLEGRO5_INCLUDE_DIRS and ALLEGRO5_LIBRARIES variables only in the advanced view
mark_as_advanced(ALLEG_INCLUDE ALLEGRO5_LIBRARIES)

