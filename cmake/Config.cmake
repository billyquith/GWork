
## Configuration options

# setup version numbers
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)
set(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message("Project version: ${VERSION_STR}")

# Windows only
if(WIN32)
    option(BUILD_RENDER_DIRECT2D    "Renderer: Direct2D" OFF)
    option(BUILD_RENDER_DIRECTX9    "Renderer: DirectX9" OFF)
    option(BUILD_RENDER_GDIPLUS     "Renderer: GDIPlus" OFF)
endif()

# Cross-platform
option(BUILD_RENDER_ALLEGRO5    "Renderer: Allegro5" OFF)
option(BUILD_RENDER_OPENGL      "Renderer: OpenGL" OFF)
option(BUILD_RENDER_SDL2        "Renderer: SDL2" OFF)
option(BUILD_RENDER_SFML        "Renderer: SFML" OFF)
option(BUILD_RENDER_SFML2       "Renderer: SFML2" OFF)

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

# Add an option for choosing the build type (shared or static)
if(NOT BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS TRUE
        CACHE BOOL "TRUE to build Gwork as a shared library, FALSE to build it as a static library."
    )
endif()

# Set the default test build option to false
if(NOT BUILD_TEST)
    set(BUILD_TEST FALSE
        CACHE BOOL "TRUE to build the unit tests, FALSE otherwise."
    )
endif()

# define install directory for miscelleneous files
if(WIN32 AND NOT UNIX)
    set(INSTALL_MISC_DIR .)
elseif(UNIX)
    set(INSTALL_MISC_DIR share/gwork)
endif()


if (BUILD_RENDER_SDL2)
    find_package(SDL2 REQUIRED)
    message("Using SDL2")
    set(RENDERER_INC "${SDL2_INCLUDE_DIRS}")
    set(RENDERER_LIB "${SDL2_LIBRARIES}")
endif(BUILD_RENDER_SDL2)

