
#### Project Configuration

# setup version numbers
set(GWK_VERSION_MAJOR 0)
set(GWK_VERSION_MINOR 2)
set(GWK_VERSION_PATCH 0)
set(GWK_VERSION_STR "${GWK_VERSION_MAJOR}.${GWK_VERSION_MINOR}.${GWK_VERSION_PATCH}")
message("Project version: ${GWK_VERSION_STR}")

### User options

set(GWK_PLATFORM_NAME "Null")

# Windows only
if(WIN32)
    # TODO: option(WANT_RENDERER_DIRECT2D  "Renderer: Direct2D" OFF)
    # TODO: option(WANT_RENDERER_DIRECTX9  "Renderer: DirectX9" OFF)
    # TODO: option(WANT_RENDERER_GDIPLUS   "Renderer: GDIPlus" OFF)
    
    # For Windows platform:
    # set(GWK_PLATFORM_NAME "Windows")    
endif()

# Cross-platform
option(WANT_RENDERER_ALLEGRO5   "Renderer: Allegro5" OFF)
option(WANT_RENDERER_OPENGL     "Renderer: OPENGL" OFF)
option(WANT_RENDERER_SDL2       "Renderer: SDL2" OFF)
option(WANT_RENDERER_SFML2      "Renderer: SFML2" OFF)
option(WANT_RENDERER_NULL       "Renderer: Null" OFF)

option(WANT_TESTS               "Include unittests" ON)
option(WANT_SAMPLE              "Include sample" ON)

# This is for development but can be used by the user.
option(WANT_ALLOC_STATS         "Track memory allocations" OFF)

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

if(WANT_TESTS)
    message("Including tests")
endif(WANT_TESTS)
    
if(WANT_SAMPLE)
    message("Including sample")
endif(WANT_SAMPLE)


if(WANT_RENDERER_ALLEGRO5)
    find_package(Allegro5 REQUIRED)
    set(GWK_RENDERER_NAME "Allegro5")
    set(GWK_RENDER_INCLUDES "${ALLEGRO5_INCLUDE_DIRS}")
    set(GWK_RENDER_LIBRARIES "${ALLEGRO5_LIBRARIES}")
    set(GWK_PLATFORM_NAME "Allegro5")
endif(WANT_RENDERER_ALLEGRO5)

if(WANT_RENDERER_OPENGL)
    find_package(GLFW REQUIRED)
    if (APPLE)
        set(GLFW_DEPENDENCIES "-framework OpenGL")
    elseif(UNIX)
        set(GLFW_DEPENDENCIES "-lGL")
    endif()
    set(GWK_RENDERER_NAME "OpenGL")
    set(GWK_RENDER_INCLUDES "${GLFW_INCLUDE_DIR}")
    set(GWK_RENDER_LIBRARIES ${GLFW_LIBRARIES} ${GLFW_DEPENDENCIES})
endif(WANT_RENDERER_OPENGL)

if(WANT_RENDERER_SDL2)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    set(GWK_RENDERER_NAME "SDL2")
    set(GWK_RENDER_INCLUDES ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
    set(GWK_RENDER_LIBRARIES ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif(WANT_RENDERER_SDL2)

if(WANT_RENDERER_SFML2)
    set(SFML_STATIC_LIBRARIES FALSE)
    find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
    if(NOT SFML_FOUND)
        message(FATAL_ERROR "SFML2 is missing components")
    endif()
    set(GWK_RENDERER_NAME "SFML2")
    set(GWK_RENDER_INCLUDES "${SFML_INCLUDE_DIR}")
    set(GWK_RENDER_LIBRARIES ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif(WANT_RENDERER_SFML2)

if(WANT_RENDERER_NULL)
    set(GWK_RENDERER_NAME "Null")
    set(GWK_RENDER_INCLUDES "")
    set(GWK_RENDER_LIBRARIES )
endif(WANT_RENDERER_NULL)


if(NOT GWK_RENDERER_NAME)
    message(FATAL_ERROR "No renderer was specified. See WANT_RENDERER_<name> options.")
endif(NOT GWK_RENDERER_NAME)

if(GWK_RENDER_INCLUDES)
    list(REMOVE_DUPLICATES GWK_RENDER_INCLUDES)
endif()
if(GWK_RENDER_LIBRARIES)
    list(REMOVE_DUPLICATES GWK_RENDER_LIBRARIES)
endif()

message("Using renderer ${GWK_RENDERER_NAME} on platform ${GWK_PLATFORM_NAME}")
message("${GWK_RENDERER_NAME} includes: ${GWK_RENDER_INCLUDES}")
message("${GWK_RENDERER_NAME} libs: ${GWK_RENDER_LIBRARIES}")
