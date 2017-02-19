
if(WANT_REFLECTION)
    add_definitions("-DGWK_REFLECT=1")
endif()

if(WANT_ALLOC_STATS)
    add_definitions("-DGWK_ALLOC_STATS=1")
endif()

add_subdirectory(platform)
add_subdirectory(gwork)
add_subdirectory(util)
add_subdirectory(test)
add_subdirectory(samples)
