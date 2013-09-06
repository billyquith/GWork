
if MONOLITHIC then
    ALLEGRO_LIBS	= { "UnitTest", "Renderer-Allegro", "GWEN-Static", "alleg" }
    ALLEGRO_LIBS_D	= { "UnitTest", "Renderer-Allegro", "GWEN-Static", "alleg" }
else
    -- Separate static libraries
    local function allib(name) return "allegro" .. name .. "-static" end

    ALLEGRO_LIBS =
    {
        -- OSX Cocoa Libraries
        "UnitTest", "Cocoa.framework", "IOKit.framework", "OpenGL.framework",
        "OpenAL.framework", "AudioToolbox.framework",

        -- Allegro dependencies
        "freetype", "bz2", "z",

        -- Allegro Libs
        allib(""), allib("_main"), allib("_ttf"), allib("_font"),
        allib("_primitives"), allib("_image"),

        -- GWEN Allegro
        "Renderer-Allegro", "GWEN-Static"
    }

    ALLEGRO_LIBS_D	= ALLEGRO_LIBS
end

SFML_LIBS		= { "UnitTest", "Renderer-SFML", "GWEN-Static", "sfml-graphics",
                    "sfml-window", "sfml-system", "GL" }
SFML_LIBS_D		= { "UnitTest", "Renderer-SFML", "GWEN-Static", "sfml-graphics",
                    "sfml-window", "sfml-system", "GL" }

SFML2_LIBS		= { "UnitTest", "Renderer-SFML2", "GWEN-Static", "sfml-graphics",
                    "sfml-window", "sfml-system", "GL" }
SFML2_LIBS_D	= { "UnitTest", "Renderer-SFML2", "GWEN-Static", "sfml-graphics",
                    "sfml-window", "sfml-system", "GL" }

CROSS_LIBS 		= { "UnitTest", "Renderer-OpenGL_DebugFont", "GWEN-Static", "GL", "freeimage" }


