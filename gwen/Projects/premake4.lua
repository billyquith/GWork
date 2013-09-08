print("Generating project files for GWEN")
print("Premake version: " .. (_PREMAKE_VERSION or "<4.4"))

dofile( "inc/include.lua" )


solution "GWEN"

    language "C++"
    location ( os.get() .. "/" .. _ACTION )
    flags { "Unicode", "Symbols",
            "NoEditAndContinue", "NoMinimalRebuild",
            "NoPCH", "EnableSSE",
            "No64BitChecks",
            
            -- Use DLL runtime libraries by default. This is preferred,
            -- and may be used by dependencies you link to. 
            --"StaticRuntime",
        }

    targetdir ( "../lib/" .. os.get() .. "/" .. _ACTION )
    libdirs { "../lib/", "../lib/" .. os.get() }

    configurations { "Release", "Debug" }

    if ( _ACTION == "vs2010" or _ACTION=="vs2008" ) then
        buildoptions {
            "/MP",  -- multi-processor compile
        }
    end

    if ( _ACTION == "xcode4" ) then
        buildoptions { "-std=c++11", "-stdlib=libc++" }
    end


configuration "Release"
    defines { "NDEBUG" }
    flags{ "Optimize", "FloatFast" }
    includedirs { "../include/" }

configuration "Debug"
    defines { "_DEBUG" }
    includedirs { "../include/" }
    targetsuffix( "d" )

project "GWEN-DLL"
    defines { "GWEN_COMPILE_DLL" }
    files { "../src/**.*", "../include/Gwen/**.*" }
    kind "SharedLib"
    targetname( "gwen" )

project "GWEN-Static"
    defines { "GWEN_COMPILE_STATIC" }
    files { "../src/**.*", "../include/Gwen/**.*" }
    flags { "Symbols" }
    kind "StaticLib"
    targetname( "gwen_static" )

project "UnitTest"
    files { "../UnitTest/**.*" }
    flags { "Symbols" }
    kind "StaticLib"
    targetname( "unittest" )

--
-- Renderers
--

DefineRenderer( "OpenGL",
                {"../Renderers/OpenGL/OpenGL.cpp"} )

DefineRenderer( "OpenGL_DebugFont",
                { "../Renderers/OpenGL/OpenGL.cpp",
                  "../Renderers/OpenGL/DebugFont/OpenGL_DebugFont.cpp" } )

DefineRenderer( "SFML",
                { "../Renderers/SFML/SFML.cpp" } )

DefineRenderer( "SFML2",
                { "../Renderers/SFML2/SFML2.cpp" } )

DefineRenderer( "Allegro",
                { "../Renderers/Allegro/Allegro.cpp" } )

DefineRenderer( "SDL2",
                { "../Renderers/SDL2/SDL2.cpp" } )

if ( os.get() == "windows" ) then
    DefineRenderer( "DirectX9",
                    { "../Renderers/DirectX9/DirectX9.cpp" } )
    includedirs { "$(DXSDK_DIR)/Include" }
    libdirs { "$(DXSDK_DIR)/lib/x86" }

    DefineRenderer( "Direct2D",
                    { "../Renderers/Direct2D/Direct2D.cpp" } )
    includedirs { "$(DXSDK_DIR)/Include" }
    libdirs { "$(DXSDK_DIR)/lib/x86" }

    DefineRenderer( "GDI",
                    { "../Renderers/GDIPlus/GDIPlus.cpp",
                      "../Renderers/GDIPlus/GDIPlusBuffered.cpp" } )
end

--
-- Samples
--

DefineSample{ name = "CrossPlatform",
              files = { "../Samples/CrossPlatform/CrossPlatform.cpp" },
              links = CROSS_LIBS,
              defs = { "USE_DEBUG_FONT" } }

DefineSample{ name = "SFML",
              files = { "../Samples/SFML/SFML.cpp" },
              links  = SFML_LIBS,
              linksd = SFML_LIBS_D }

DefineSample{ name = "SFML2",
              files = { "../Samples/SFML2/SFML2.cpp" },
              links = SFML2_LIBS,
              linksd = SFML2_LIBS_D }

DefineSample{ name = "Allegro",
              files = { "../Samples/Allegro/AllegroSample.cpp" },
              links = ALLEGRO_LIBS,
              linksd = ALLEGRO_LIBS_D }

DefineSample{ name = "SDL2",
              files = { "../Samples/SDL2/SDL2Sample.cpp" },
              links = SDL2_LIBS,
              linksd = SDL2_LIBS_D }

if ( os.get() == "windows" ) then

    DefineSample{ name = "Direct2D",
                  files = { "../Samples/Direct2D/Direct2DSample.cpp" },
                  links = { "UnitTest", "Renderer-Direct2D", "GWEN-Static", "d2d1",
                            "dwrite", "windowscodecs" } }
        includedirs { "$(DXSDK_DIR)/Include" }
        libdirs { "$(DXSDK_DIR)/lib/x86" }

    DefineSample{ name = "DirectX9",
                  files = { "../Samples/Direct3D/Direct3DSample.cpp" },
                  links = { "UnitTest", "Renderer-DirectX9", "GWEN-Static" } }
        includedirs { "$(DXSDK_DIR)/Include" }
        libdirs { "$(DXSDK_DIR)/lib/x86" }

    DefineSample{ name = "WindowsGDI",
                  files = { "../Samples/WindowsGDI/WindowsGDI.cpp" },
                  links = { "UnitTest", "Renderer-GDI", "GWEN-Static" } }

    DefineSample{ name = "OpenGL",
                  files = { "../Samples/OpenGL/OpenGLSample.cpp" },
                  links = { "UnitTest", "Renderer-OpenGL", "GWEN-Static", "FreeImage", "opengl32" } }

    DefineSample{ name = "OpenGL_DebugFont",
                  files = { "../Samples/OpenGL/OpenGLSample.cpp" },
                  links = { "UnitTest", "Renderer-OpenGL_DebugFont", "GWEN-Static", "FreeImage", "opengl32" },
                  defs = { "USE_DEBUG_FONT" } }

end

project "ControlFactory"
    files { "../Util/ControlFactory/**.*" }
    kind "StaticLib"
    targetname( "controlfactory" )

project "ImportExport"
    files { "../Util/ImportExport/**.*" }
    kind "StaticLib"
    targetname( "importexport" )
