# Gwork dependencies

if(WANT_REFLECTION)
    if (WANT_REFLECTION_LOCAL)
        message(STATUS "Using project local Ponder")
        add_subdirectory(ponder)        
        add_subdirectory(lua-5.3)
    endif()    
endif()
