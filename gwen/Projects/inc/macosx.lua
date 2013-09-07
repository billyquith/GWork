
if user.al_monolithic then
    ALLEGRO_LIBS	= { "UnitTest", "Renderer-Allegro", "GWEN-Static", "alleg" }
    ALLEGRO_LIBS_D	= { "UnitTest", "Renderer-Allegro", "GWEN-Static", "alleg" }
else
    -- Separate static libraries
    local function allib(name) return "allegro" .. name .. "-static" end

    ALLEGRO_LIBS =
    {
        -- OSX Cocoa Libraries
        "Cocoa.framework", "IOKit.framework", "OpenGL.framework",
        "OpenAL.framework", "AudioToolbox.framework",

        -- Allegro dependencies
        "freetype", "bz2", "z",

        -- Allegro Libs
        allib(""), allib("_main"), allib("_ttf"), allib("_font"),
        allib("_primitives"), allib("_image"),

        -- GWEN Allegro
        "Renderer-Allegro", "GWEN-Static", "UnitTest"
    }

    ALLEGRO_LIBS_D	= ALLEGRO_LIBS
end

CROSS_LIBS 		= { "GL", "freeimage",
                    "Renderer-OpenGL_DebugFont", "GWEN-Static",
                    "UnitTest" }

SDL2_LIBS = {
        -- OSX Cocoa Libraries
        "Cocoa.framework", "IOKit.framework",
        "OpenGL.framework", "ForceFeedback.framework",
        "CoreAudio.framework", "AudioUnit.framework", "AudioToolbox.framework",
        "Carbon.framework",  -- keyboard (hmm)

        -- Note: Macports version of this won't compile as x86_64.
        -- See: http://blog.wwwjfy.net/2011/03/02/compile-vim-7-3-in-mac-os-x-10-6-6/
        "iconv",

        -- SDL2 libraries.
        -- You can get these from Macports:
        --  port install libsdl2 libsdl2_image libsdl2_ttf
        -- Add "+universal" if you'd like 32/64 bit versions.
        "SDL2", "SDL2_image", "SDL2_ttf",

        "Renderer-SDL2", "GWEN-Static", "UnitTest"
        }

SFML_LIBS		= { "GL",
                    "sfml-window", "sfml-system", "sfml-graphics",
                    "Renderer-SFML", "GWEN-Static",
                    "UnitTest" }

SFML2_LIBS		= { "GL",
                    "sfml-graphics", "sfml-window", "sfml-system",
                    "Renderer-SFML2", "GWEN-Static",
                    "UnitTest" }



