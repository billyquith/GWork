# Gwork platform

include_directories(
    ${CMAKE_SOURCE_DIR}/source/platform/include
    ${RENDERER_INC}
    ${CMAKE_SOURCE_DIR}/source/gwork/include        # TODO: remove
)

 set(GWK_RENDER_SOURCES
     # headers
     include/Gwork/Exports.h
     include/Gwork/Platform.h
     include/Gwork/Renderers/${RENDERER_NAME}.h
     # source
     renderers/${RENDERER_NAME}/${RENDERER_NAME}.cpp
     platforms/${BUILD_PLATFORM}.cpp
 )

# Gwork renderer & platform library
add_library(Gwork${RENDERER_NAME} STATIC ${GWK_RENDER_SOURCES})
target_compile_features(Gwork${RENDERER_NAME} PUBLIC cxx_override)
target_link_libraries(Gwork${RENDERER_NAME} Gwork)

install(FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork/Exports.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork/Platform.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/Gwork/Renderers/${RENDERER_NAME}.h
        DESTINATION include/Gwork/Renderers)

install(TARGETS Gwork${RENDERER_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)      
