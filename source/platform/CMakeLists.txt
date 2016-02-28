# Gwork platform

include_directories(
    ${CMAKE_SOURCE_DIR}/source/platform/include
    ${RENDERER_INC}
    # ${CMAKE_SOURCE_DIR}/source/gwork/include        # TODO: remove
)

set(GWK_RENDER_HEADERS
    include/Gwork/BaseRender.h
    include/Gwork/Exports.h
    include/Gwork/Platform.h
    include/Gwork/PlatformTypes.h
    include/Gwork/Renderers/${RENDERER_NAME}.h
)

set(GWK_RENDER_SOURCES
    renderers/${RENDERER_NAME}/${RENDERER_NAME}.cpp
    platforms/${BUILD_PLATFORM}.cpp
)

# Gwork renderer & platform library
add_library(Gwork${RENDERER_NAME} STATIC ${GWK_RENDER_HEADERS} ${GWK_RENDER_SOURCES})
target_compile_features(Gwork${RENDERER_NAME} PUBLIC cxx_override)
target_link_libraries(Gwork${RENDERER_NAME} Gwork)

install(FILES ${GWK_RENDER_HEADERS}
        DESTINATION include/Gwork/Renderers)

install(TARGETS Gwork${RENDERER_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)      
