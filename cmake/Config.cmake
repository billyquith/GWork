
#### Project Configuration

# Version
set(GWK_VERSION_MAJOR 0)
set(GWK_VERSION_MINOR 2)
set(GWK_VERSION_PATCH 0)
set(GWK_BRANCH "Dev")       # Dev or Release
set(GWK_VERSION_STR "${GWK_VERSION_MAJOR}.${GWK_VERSION_MINOR}.${GWK_VERSION_PATCH} ${GWK_BRANCH}")
message("Project version: ${GWK_VERSION_STR}")

#-----------------------------------------------------------
# User options

# Windows only
if(WIN32)
    # TODO: Direct2D and GDI+ are partially working - use cross platform, or fix!
    # option(RENDER_DIRECT2D  "Renderer: Direct2D" OFF)
    # TODO: option(RENDER_DIRECTX9  "Renderer: DirectX9" OFF)
    # option(RENDER_GDIPLUS   "Renderer: GDIPlus" OFF)    
endif()

# Cross-platform
option(RENDER_ALLEGRO5      "Renderer: Allegro5" OFF)
option(RENDER_OPENGL        "Renderer: OPENGL" OFF)
option(RENDER_SDL2          "Renderer: SDL2" OFF)
option(RENDER_SFML2         "Renderer: SFML2" OFF)
option(RENDER_NULL          "Renderer: Null" OFF)       # Used for testing

option(BUILD_TEST           "Include unittests" ON)
option(BUILD_SAMPLE         "Include sample" ON)

# This is for development but can be used by the user.
option(ALLOC_STATS "Track memory allocations" OFF)

#-----------------------------------------------------------
# Configure once options known

set(GWK_PLATFORM_NAME "Null")   # default/fallback platform

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
set(BUILD_SHARED_LIBS FALSE)

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

#-----------------------------------------------------------
# Renderer config
#   - Order alphabetic.

if(RENDER_ALLEGRO5)
    set(GWK_RENDER_NAME "Allegro5")
    find_package(Allegro5 REQUIRED)
    set(GWK_RENDER_INCLUDES "${ALLEGRO5_INCLUDE_DIRS}")
    set(GWK_RENDER_LIBRARIES "${ALLEGRO5_LIBRARIES}")
    set(GWK_PLATFORM_NAME "Allegro5")
endif(RENDER_ALLEGRO5)

if(RENDER_GDIPLUS)
    set(GWK_RENDER_NAME "GDIPlus")
    find_package(GDIPLUS REQUIRED)
    set(GWK_RENDER_INCLUDES "${GDIPLUS_INCLUDE_DIR}")
    set(GWK_RENDER_LIBRARIES "${GDIPLUS_LIBRARIES}")
    set(GWK_INPUT_NAME "Windows")
endif(RENDER_GDIPLUS)

if(RENDER_DIRECT2D)
    set(GWK_RENDER_NAME "Direct2D")
    find_package(Direct2D REQUIRED)
    set(GWK_RENDER_INCLUDES "${DIRECT2D_INCLUDE_DIRS}")
    set(GWK_RENDER_LIBRARIES "${DIRECT2D_LIBRARIES}")
    set(GWK_INPUT_NAME "Windows")
endif(RENDER_DIRECT2D)

if(RENDER_NULL)
    set(GWK_RENDER_NAME "Null")
    set(GWK_RENDER_INCLUDES "")
    set(GWK_RENDER_LIBRARIES )
endif(RENDER_NULL)

if(RENDER_OPENGL)
    find_package(GLFW REQUIRED)
    if (APPLE)
        set(GLFW_DEPENDENCIES "-framework OpenGL")
    elseif(UNIX)
        set(GLFW_DEPENDENCIES "-lGL")
    elseif(WIN32)
        find_package(OpenGL)
        set(GLFW_DEPENDENCIES ${OPENGL_gl_LIBRARY})
    endif()
    set(GWK_RENDER_NAME "OpenGL")
    set(GWK_RENDER_INCLUDES "${GLFW_INCLUDE_DIR}")
    set(GWK_RENDER_LIBRARIES ${GLFW_LIBRARIES} ${GLFW_DEPENDENCIES})
endif(RENDER_OPENGL)

if(RENDER_SDL2)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    set(GWK_RENDER_NAME "SDL2")
    set(GWK_RENDER_INCLUDES ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
    set(GWK_RENDER_LIBRARIES ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif(RENDER_SDL2)

if(RENDER_SFML2)
    set(SFML_STATIC_LIBRARIES FALSE)
    find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
    if(NOT SFML_FOUND)
        message(FATAL_ERROR "SFML2 is missing components")
    endif()
    set(GWK_RENDER_NAME "SFML2")
    set(GWK_RENDER_INCLUDES "${SFML_INCLUDE_DIR}")
    set(GWK_RENDER_LIBRARIES ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif(RENDER_SFML2)

#-----------------------------------------------------------
# Sanity checks and summary

if(NOT GWK_INPUT_NAME)
    set(GWK_INPUT_NAME ${GWK_RENDER_NAME})
endif()

if(NOT GWK_RENDER_NAME)
    message(FATAL_ERROR "No renderer was specified. See RENDER_<name> options.")
endif(NOT GWK_RENDER_NAME)

if(GWK_RENDER_INCLUDES)
    list(REMOVE_DUPLICATES GWK_RENDER_INCLUDES)
endif()
if(GWK_RENDER_LIBRARIES)
    list(REMOVE_DUPLICATES GWK_RENDER_LIBRARIES)
endif()

message("Using renderer ${GWK_RENDER_NAME} on platform ${GWK_PLATFORM_NAME}")
message("${GWK_RENDER_NAME} includes: ${GWK_RENDER_INCLUDES}")
message("${GWK_RENDER_NAME} libs: ${GWK_RENDER_LIBRARIES}")
