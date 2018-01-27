# Gwork dependencies

if(WANT_REFLECTION)
    if (WANT_REFLECTION_LOCAL)
        message(STATUS "Using project local Ponder")
        
        set(BUILD_SHARED_LIBS OFF)
        add_subdirectory(ponder)
        
        add_subdirectory(lua-5.3)
    endif()    
endif()
