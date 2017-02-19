# Gwork platform

include_directories(
    ${GWK_SOURCE_DIR}/source/platform/include
    ${GWK_RENDER_INCLUDES}
)

set(GWK_RENDER_HEADERS
    include/Gwork/BaseRender.h
    include/Gwork/Config.h
    include/Gwork/InputEventListener.h
    include/Gwork/PlatformTypes.h
    include/Gwork/Platform.h
    include/Gwork/PlatformCommon.h
    include/Gwork/Renderers/${GWK_RENDERER_NAME}.h
    include/Gwork/Input/${GWK_RENDERER_NAME}.h
)

set(GWK_RENDER_SOURCES
    renderers/${GWK_RENDERER_NAME}/${GWK_RENDERER_NAME}.cpp
    platforms/${GWK_PLATFORM_NAME}Platform.cpp
    platforms/PlatformCommon.cpp
)

source_group("${GWK_HEADER_FOLDER}\\Input"
             REGULAR_EXPRESSION "include/Gwork/Input/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Renderer"
             REGULAR_EXPRESSION "include/Gwork/Renderers/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}"
             REGULAR_EXPRESSION "include/Gwork/[^/]+\\.h")

# source folder groups
source_group("${GWK_SOURCE_FOLDER}"
             REGULAR_EXPRESSION ".*\\.cpp")

# Gwork renderer & platform library
add_library(Gwork${GWK_RENDERER_NAME} STATIC ${GWK_RENDER_HEADERS} ${GWK_RENDER_SOURCES})
target_link_libraries(Gwork${GWK_RENDERER_NAME} Gwork)

install(FILES ${GWK_RENDER_HEADERS}
        DESTINATION include/Gwork/Input)

install(FILES ${GWK_RENDER_HEADERS}
        DESTINATION include/Gwork/Renderers)

install(TARGETS Gwork${GWK_RENDERER_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)      
