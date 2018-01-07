# Gwork projects

if(WANT_REFLECTION)
    if (WANT_REFLECTION_LOCAL)
        message(STATUS "Using local deps/ Ponder")
        add_subdirectory(deps/ponder)
        set(PONDER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/ponder/include)
        set(PONDER_LIBRARIES)
    else()
        find_package(Ponder 2.2 REQUIRED)
    endif()
    
    message(STATUS "Including Ponder from ${PONDER_INCLUDE_DIR}")

    find_package(Lua 5.3 REQUIRED)
    message(STATUS "Including Lua from ${LUA_INCLUDE_DIR}")
    
    add_definitions("-DGWK_REFLECT=1")
    add_definitions("-DPONDER_USING_LUA=1")
    
    set(GWK_REFLECT_INCLUDE ${PONDER_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
    list(REMOVE_DUPLICATES GWK_REFLECT_INCLUDE)
    set(GWK_REFLECT_LIBRARIES ${PONDER_LIBRARIES} ${LUA_LIBRARIES})
    message(STATUS "Relection include: ${GWK_REFLECT_INCLUDE}. Libs: ${GWK_REFLECT_LIBRARIES}")
endif()

if(WANT_ALLOC_STATS)
    add_definitions("-DGWK_ALLOC_STATS=1")
endif()

add_subdirectory(platform)
add_subdirectory(gwork)
add_subdirectory(util)
add_subdirectory(test)
add_subdirectory(samples)
