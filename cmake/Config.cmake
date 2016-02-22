
#### Project Configuration

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message("Project version: ${VERSION_STR}")

### User options

# Windows only
if(WIN32)
    # TODO: option(RENDER_DIRECT2D  "Renderer: Direct2D" OFF)
    option(RENDER_DIRECTX9  "Renderer: DirectX9" OFF)
    option(RENDER_GDIPLUS   "Renderer: GDIPlus" OFF)
endif()

# Cross-platform
option(RENDER_ALLEGRO5      "Renderer: Allegro5" OFF)
option(RENDER_OPENGL        "Renderer: OpenGL" OFF)
option(RENDER_SDL2          "Renderer: SDL2" OFF)
# TODO: option(RENDER_SFML          "Renderer: SFML" OFF)
option(RENDER_SFML2         "Renderer: SFML2" OFF)

option(BUILD_TEST           "Include unittests" ON)
option(BUILD_SAMPLE         "Include sample" ON)


if(WIN32)
    set(BUILD_PLATFORM "Windows")
else()
    set(BUILD_PLATFORM "Null")
endif()

# Set the default build type to release with debug info
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo
        CACHE STRING "Type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    )
endif()

# TODO: Add an option for choosing the build type (shared or static)
# if(NOT BUILD_SHARED_LIBS)
#     set(BUILD_SHARED_LIBS TRUE
#         CACHE BOOL "TRUE to build Gwork as a shared library, FALSE to build it as a static library."
#     )
# endif()

# define install directory for miscelleneous files
if(WIN32 AND NOT UNIX)
    set(INSTALL_MISC_DIR .)
elseif(UNIX)
    set(INSTALL_MISC_DIR share/gwork)
endif()

if(BUILD_TEST)
    message("Including tests")
endif(BUILD_TEST)
    
if(BUILD_SAMPLE)
    message("Including sample")
endif(BUILD_SAMPLE)

if(RENDER_SDL2)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    set(RENDERER_NAME "SDL2")
    set(RENDERER_INC ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
    set(RENDERER_LIB ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif(RENDER_SDL2)

if(RENDER_SFML2)
    find_package(SFML2 REQUIRED)
    set(RENDERER_NAME "SFML2")
    set(RENDERER_INC "${SFML2_INCLUDE_DIRS}")
    set(RENDERER_LIB "${SFML2_LIBRARIES}")
endif(RENDER_SFML2)

message("Using renderer ${RENDERER_NAME}")
