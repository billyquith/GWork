# Gwork dependencies

if(WITH_REFLECTION)
    if (WITH_REFLECTION_LOCAL)
        message(STATUS "Using project local Ponder")

        set(BUILD_SHARED_LIBS OFF)
        add_subdirectory(ponder)
    endif()
endif()
