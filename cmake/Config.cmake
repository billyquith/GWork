
#### Gwork Project Configuration

# Version
set(GWK_VERSION_MAJOR 1)
set(GWK_VERSION_MINOR 0)
set(GWK_VERSION_PATCH 0)
set(GWK_BRANCH "Release")       # Dev or Release
set(GWK_VERSION_STR "${GWK_VERSION_MAJOR}.${GWK_VERSION_MINOR}.${GWK_VERSION_PATCH} ${GWK_BRANCH}")
message("Project version: ${GWK_VERSION_STR}")

#-----------------------------------------------------------
# User options

# Windows only
if(WIN32)
    # option(RENDER_DIRECTX9  "Renderer: DirectX9" OFF)
    option(RENDER_DIRECTX11  "Renderer: DirectX11" OFF)
endif()

# Cross-platform
option(RENDER_ALLEGRO5      "Renderer: Allegro5"            OFF)
option(RENDER_IRRLICHT      "Renderer: Irrlicht"            OFF)
option(RENDER_OPENGL        "Renderer: OPENGL"              OFF)
option(RENDER_OPENGL_CORE   "Renderer: OPENGL Core Profile" OFF)
option(RENDER_SDL2          "Renderer: SDL2"                OFF)
option(RENDER_SFML2         "Renderer: SFML2"               OFF)
option(RENDER_SW            "Renderer: Software"            OFF)
option(RENDER_NULL          "Renderer: Null"                OFF) # Used for testing

option(USE_GLFW             "Use GLFW for OpenGL renderer." ON)

option(WITH_TESTS           "Include unittests" ON)
option(WITH_SAMPLE          "Include sample" ON)

option(WITH_REFLECTION      "Use reflection (requires external dependencies)" OFF)

# This is for development but can be used by the user.
option(WITH_ALLOC_STATS     "Track memory allocations" OFF)

# Find SFML by SFMLConfig.cmake instead of local CMake module FindSFML.cmake.
option(USE_SFML_CONFIG "Use SFMLConfig.cmake to find SFML (>=2.5)" OFF)

#-----------------------------------------------------------
# Configure once options known

option(WANT_SHARED_LIBS OFF) # TODO: Shared libs not implemented yet.

option(WITH_REFLECTION_LOCAL "Use local libaries" ON) # TODO: OFF not tested.
set(GWK_PONDER_INCLUDE "${GWK_SOURCE_DIR}/deps/ponder/include"
    CACHE STRING "Ponder includes directory")
set(GWK_LUA_INCLUDE "${GWK_SOURCE_DIR}/deps/lua-5.3/src"
    CACHE STRING "Lua include directory")

set(GWK_TARGET_ARCH "Unknown")      # default architecture e.g. x86, x64

# Set the default build type to release with debug info
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo
        CACHE STRING "Type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
    )
endif()

# define install directory for miscelleneous files
if(WIN32 AND NOT UNIX)
    set(INSTALL_MISC_DIR .)

    # Determine the target architecture, which is useful for linking.
    if (CMAKE_GENERATOR MATCHES "Win64")
        set(GWK_TARGET_ARCH "x64")
    else()
        set(GWK_TARGET_ARCH "x86")
    endif()
    message(STATUS "Generator: ${CMAKE_GENERATOR}. Architecture: ${GWK_TARGET_ARCH}")
elseif(UNIX)
    set(INSTALL_MISC_DIR share/gwork)
endif()

if(WITH_TESTS)
    message("Including tests")
endif(WITH_TESTS)

if(WITH_SAMPLE)
    message(STATUS "Including Gwk sample")

    if (NOT USE_GLFW AND (RENDER_OPENGL OR RENDER_OPENGL_CORE))
        message(FATAL_ERROR "Samples with OpenGL or OpenGLCore require GLFW")
    endif()
endif(WITH_SAMPLE)

if(WITH_REFLECTION)
    message("Using reflection")
endif(WITH_REFLECTION)

#-----------------------------------------------------------
# Renderer config
#   - Order alphabetic.

if(RENDER_ALLEGRO5)
    set(GWK_RENDER_NAME "Allegro5")
    set(GWK_INPUT_NAME "Allegro5")
    set(GWK_PLATFORM_NAME "Allegro5")
	find_package(Allegro5 REQUIRED)
	set(GWK_RENDER_INCLUDES "${ALLEGRO5_INCLUDE_DIRS}")
	set(GWK_RENDER_LIBRARIES "${ALLEGRO5_LIBRARIES}")
endif(RENDER_ALLEGRO5)

if(RENDER_DIRECTX9)
    set(GWK_RENDER_NAME "DirectX9")
    set(GWK_INPUT_NAME "Windows")
    set(GWK_PLATFORM_NAME "Windows")
    find_package(DirectX REQUIRED)
    set(GWK_RENDER_INCLUDES "${DIRECTX_INCLUDE_DIRS}")
    set(GWK_RENDER_LIBRARIES "${DIRECTX_LIBRARIES}")
endif(RENDER_DIRECTX9)

if(RENDER_DIRECTX11)
    set(GWK_RENDER_NAME "DirectX11")
    set(GWK_INPUT_NAME "Windows")
    set(GWK_PLATFORM_NAME "Windows")
    find_package(DirectX11 REQUIRED)
    set(GWK_RENDER_INCLUDES "${DIRECTX_INCLUDE_DIRS}")
    set(GWK_RENDER_LIBRARIES "${DIRECTX_LIBRARIES}")
endif(RENDER_DIRECTX11)

if(RENDER_IRRLICHT)
    set(GWK_RENDER_NAME "Irrlicht")
    set(GWK_INPUT_NAME "Irrlicht")
    set(GWK_PLATFORM_NAME "Cross")
    find_package(Irrlicht REQUIRED)
    if (APPLE)
        set(IRR_DEPENDENCIES "-framework OpenGL")
    elseif(UNIX)
        set(IRR_DEPENDENCIES "-lGL")
    endif()
    set(GWK_RENDER_INCLUDES "${IRRLICHT_INCLUDE_DIR}")
    set(GWK_RENDER_LIBRARIES "${IRRLICHT_LIBRARY} ${IRR_DEPENDENCIES}")
endif(RENDER_IRRLICHT)

if(RENDER_NULL)
    set(GWK_RENDER_NAME "Null")
    set(GWK_INPUT_NAME "Null")
    set(GWK_PLATFORM_NAME "Null")
    set(GWK_RENDER_INCLUDES "")
    set(GWK_RENDER_LIBRARIES "")
endif(RENDER_NULL)

if(RENDER_OPENGL)
    set(GWK_RENDER_NAME "OpenGL")
    set(GWK_INPUT_NAME "GLFW3")
    set(GWK_PLATFORM_NAME "Cross")

    if (USE_GLFW)
        message(STATUS "Configuring GLFW3...")
        find_package(GLFW REQUIRED)
        if (APPLE)
            set(GLFW_DEPENDENCIES "-framework OpenGL")
        elseif(UNIX)
            set(GLFW_DEPENDENCIES "-lGL")
        elseif(WIN32)
            find_package(OpenGL)
            set(GLFW_DEPENDENCIES ${OPENGL_gl_LIBRARY})
        endif()

        set(GWK_RENDER_INCLUDES "${GLFW_INCLUDE_DIR}")
        set(GWK_RENDER_LIBRARIES ${GLFW_LIBRARIES} ${GLFW_DEPENDENCIES})
    endif()
endif(RENDER_OPENGL)

if(RENDER_OPENGL_CORE)
    set(GWK_RENDER_NAME "OpenGLCore")
    set(GWK_INPUT_NAME "GLFW3")
    set(GWK_PLATFORM_NAME "Cross")

    find_package(glm REQUIRED)
    find_package(GLEW REQUIRED)

    set(GWK_RENDER_INCLUDES "${GLM_INCLUDE_DIR}" "${GLEW_INCLUDE_DIR}")
    set(GWK_RENDER_LIBRARIES ${GLM_LIBRARIES} ${GLEW_LIBRARIES})

    if(USE_GLFW)
        find_package(GLFW REQUIRED)
        if (APPLE)
            set(GLFW_DEPENDENCIES "-framework OpenGL")
        elseif(UNIX)
            set(GLFW_DEPENDENCIES "-lGL")
        elseif(WIN32)
            find_package(OpenGL)
            set(GLFW_DEPENDENCIES ${OPENGL_gl_LIBRARY})
        endif()

        set(GWK_RENDER_INCLUDES "${GWK_RENDER_INCLUDES}" "${GLFW_INCLUDE_DIR}")
        set(GWK_RENDER_LIBRARIES ${GWK_RENDER_LIBRARIES} ${GLFW_LIBRARIES} ${GLFW_DEPENDENCIES})
    endif()
endif()

if(RENDER_SDL2)
    set(GWK_RENDER_NAME "SDL2")
    set(GWK_INPUT_NAME "SDL2")
    set(GWK_PLATFORM_NAME "Cross")
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)
    set(GWK_RENDER_INCLUDES ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
    set(GWK_RENDER_LIBRARIES ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES})
endif(RENDER_SDL2)

if(RENDER_SFML2)
    set(GWK_RENDER_NAME "SFML2")
    set(GWK_INPUT_NAME "SFML2")
    set(GWK_PLATFORM_NAME "Cross")
    set(SFML_STATIC_LIBRARIES FALSE) # But note we edited FindSFML...
    find_package(OpenGL REQUIRED)
    if(USE_SFML_CONFIG)
        find_package(SFML 2 CONFIG COMPONENTS system window graphics REQUIRED)
        set(GWK_RENDER_INCLUDES ${OPENGL_INCLUDE_DIRS})
        set(GWK_RENDER_LIBRARIES ${OPENGL_LIBRARIES} sfml-system sfml-window sfml-graphics)
    else()
        find_package(SFML 2 COMPONENTS system window graphics REQUIRED)
        set(GWK_RENDER_INCLUDES ${OPENGL_INCLUDE_DIRS} ${SFML_INCLUDE_DIR})
        set(GWK_RENDER_LIBRARIES ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
    endif()
    if(NOT SFML_FOUND)
        message(FATAL_ERROR "SFML2 is missing components")
    endif()
endif(RENDER_SFML2)

if(RENDER_SW)
    set(SW_VIEWER 1)
    set(GWK_RENDER_NAME "Software")
    set(GWK_PLATFORM_NAME "Cross")
    if(SW_VIEWER)
        set(GWK_INPUT_NAME "SDL2")
        find_package(SDL2 REQUIRED)
        set(GWK_RENDER_INCLUDES ${SDL2_INCLUDE_DIR})
        set(GWK_RENDER_LIBRARIES ${SDL2_LIBRARY})
    else()
        set(GWK_INPUT_NAME "Null")
        set(GWK_RENDER_INCLUDES "")
        set(GWK_RENDER_LIBRARIES "")
    endif()
endif(RENDER_SW)

#-----------------------------------------------------------
# Sanity checks

if(WANT_SHARED_LIBS)
    message(FATAL_ERROR "Shared libraries not currently tested/supported")
endif()

if(NOT GWK_RENDER_NAME)
    message(FATAL_ERROR "No renderer was specified. See RENDER_<name> options.")
endif(NOT GWK_RENDER_NAME)

if(NOT GWK_INPUT_NAME)
    message(FATAL_ERROR "No GWK_INPUT_NAME specified")
endif()

if(NOT GWK_PLATFORM_NAME)
    message(FATAL_ERROR "No GWK_PLATFORM_NAME specified")
endif()

#-----------------------------------------------------------

# MinGW problems
if (WIN32)
    set(GWK_RENDER_LIBRARIES ${GWK_RENDER_LIBRARIES} -liconv)
endif()

set(GWK_LIB_DEFINES "-DGWK_PLATFORM_${GWK_PLATFORM_NAME}=1 -DGWK_RENDER_${GWK_RENDER_NAME}=1")

if(GWK_RENDER_INCLUDES)
    list(REMOVE_DUPLICATES GWK_RENDER_INCLUDES)
endif()
if(GWK_RENDER_LIBRARIES)
    list(REMOVE_DUPLICATES GWK_RENDER_LIBRARIES)
endif()

# Summary
message(STATUS "Using renderer '${GWK_RENDER_NAME}' with input '${GWK_INPUT_NAME}' on platform '${GWK_PLATFORM_NAME}'")
message(STATUS "${GWK_RENDER_NAME} includes: ${GWK_RENDER_INCLUDES}")
message(STATUS "${GWK_RENDER_NAME} libs: ${GWK_RENDER_LIBRARIES}")
