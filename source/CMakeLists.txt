# Gwork projects

if(WANT_REFLECTION)    
    set(GWK_REFLECT_INCLUDE ${GWK_PONDER_INCLUDE} ${GWK_LUA_INCLUDE})
    list(REMOVE_DUPLICATES GWK_REFLECT_INCLUDE)
    if (WANT_REFLECTION_LOCAL)
        set(GWK_REFLECT_LIBRARIES ponder lua)
    else()
        set(GWK_REFLECT_LIBRARIES ${GWK_PONDER_LIBRARIES} ${GWK_LUA_LIBRARIES})
    endif()
    message(STATUS "Relection include: ${GWK_REFLECT_INCLUDE}. Libs: ${GWK_REFLECT_LIBRARIES}")
    
    add_definitions("-DGWK_REFLECT=1")      # required by Gwork
    add_definitions("-DPONDER_USING_LUA=1") # required by Ponder    
endif()

if(WANT_ALLOC_STATS)
    add_definitions("-DGWK_ALLOC_STATS=1")
endif()

add_subdirectory(platform)
add_subdirectory(gwork)
add_subdirectory(util)
add_subdirectory(test)
add_subdirectory(samples)
