# Gwork platform

set(GWK_RENDERER_HEADERS
    include/Gwork/Renderers/${GWK_RENDER_NAME}.h
)

set(GWK_INPUT_HEADERS
    include/Gwork/Input/${GWK_INPUT_NAME}.h
)

set(GWK_PLATFORM_HEADERS
    include/Gwork/BaseRender.h
    include/Gwork/Config.h
    include/Gwork/InputEventListener.h
    include/Gwork/PlatformTypes.h
    include/Gwork/Platform.h
    include/Gwork/PlatformCommon.h
    include/Gwork/Version.h             # Auto-generated
    include/Gwork/WindowProvider.h
    include/Gwork/Utility.h
)

set(GWK_PLATFORM_SOURCES
    renderers/${GWK_RENDER_NAME}/${GWK_RENDER_NAME}.cpp
    platforms/${GWK_PLATFORM_NAME}Platform.cpp
    platforms/PlatformCommon.cpp
    platforms/Utility.cpp
    platforms/DebugBreak.h
)

source_group("${GWK_HEADER_FOLDER}\\Input"
             REGULAR_EXPRESSION "include/Gwork/Input/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}\\Renderer"
             REGULAR_EXPRESSION "include/Gwork/Renderers/[^/]+\\.h")
source_group("${GWK_HEADER_FOLDER}"
             REGULAR_EXPRESSION "include/Gwork/[^/]+\\.h")

# source folder groups
source_group("${GWK_SOURCE_FOLDER}"
             FILES
                 "platforms/DebugBreak.h"
             REGULAR_EXPRESSION
                 ".*\\.cpp")

add_definitions(${GWK_LIB_DEFINES})

# Gwork renderer & platform library
add_library(Gwork${GWK_RENDER_NAME} STATIC
            ${GWK_PLATFORM_HEADERS} ${GWK_RENDERER_HEADERS} ${GWK_INPUT_HEADERS}
            ${GWK_PLATFORM_SOURCES})

target_include_directories(Gwork${GWK_RENDER_NAME} PUBLIC
        ${GWK_SOURCE_DIR}/source/platform/include
        ${GWK_RENDER_INCLUDES})

target_link_libraries(Gwork${GWK_RENDER_NAME} PRIVATE ${GWK_RENDER_LIBRARIES})

install(FILES ${GWK_PLATFORM_HEADERS}
        DESTINATION include/Gwork)

install(FILES ${GWK_RENDERER_HEADERS}
        DESTINATION include/Gwork/Renderers)

install(FILES ${GWK_INPUT_HEADERS}
        DESTINATION include/Gwork/Input)

install(TARGETS Gwork${GWK_RENDER_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
