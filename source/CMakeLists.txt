
if(WANT_REFLECTION)
    find_package(Ponder REQUIRED)
    message(STATUS "Including Ponder from ${PONDER_INCLUDE_DIR}")

    find_package(Lua 5.2 REQUIRED)
    message(STATUS "Including Lua from ${LUA_INCLUDE_DIR}")
    
    add_definitions("-DGWK_REFLECT=1")
    add_definitions("-DPONDER_USING_LUA=1")
    
    set(GWK_REFLECT_INCLUDE ${PONDER_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
    list(REMOVE_DUPLICATES GWK_REFLECT_INCLUDE)
    message(STATUS "Relection include: ${GWK_REFLECT_INCLUDE}")    
endif()

if(WANT_ALLOC_STATS)
    add_definitions("-DGWK_ALLOC_STATS=1")
endif()

add_subdirectory(platform)
add_subdirectory(gwork)
add_subdirectory(util)
add_subdirectory(test)
add_subdirectory(samples)
