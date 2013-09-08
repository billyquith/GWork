
if not _ACTION then
    -- Docs: http://industriousone.com/command-line-arguments
    error("No target specified, e.g. vs2010 or xcode4")
end

--
-- Try to read the options user options.
-- This contains a table "user".
--
local user_config_options = '../user.lua'
if ( io.open(user_config_options) ) then
    print("Reading user options")
    dofile(user_config_options)
else
    print("No user options. Writing defaults.")
    local fh = io.open(user_config_options,'w')
    fh:write([[

-- User overrides. Comment in to use, otherwise ignored.
user = {
    --- User include/lib paths. E.g. look for dependencies
    --- Note: Use "/" for Windows, not "\".
    --includes = { "/my/path" },    -- User include paths.
    --libs = { "/my/path" },        -- User lib paths.

    --- Options. These are ignored if not applicable.
    --macports = true,       -- Use Macports on OSX.
    --al_monolithic = true,  -- Link to monolithic lib for Alllegro5.
}

]])
    fh:close()
    dofile(user_config_options)
end

local c_platform = os.get()
print("Platform: " .. c_platform)
dofile( c_platform .. ".lua" )


function DefineRenderer( name, filetable )

	project ( "Renderer-"..name )
	files( filetable )
	flags( { "Symbols" } )
	kind( "StaticLib" )

    if ( os.get() == "macosx" ) then
        if user.macports then
            includedirs { "/opt/local/include" }
        end
    end

    if ( user.includes ) then
        includedirs(user.includes)
    end

	configuration( "Release" )
		targetname( "GWEN-Renderer-"..name )

	configuration( "Debug" )
		targetname( "GWEND-Renderer-"..name )
end

function DefineSample(opt)
    -- options:  name, files, links(d), defs

    local prjname = "Sample-" .. opt.name
    print("Adding " .. prjname)

	project( prjname )
	targetdir ( "../bin" )

	if ( debugdir ) then
		debugdir ( "../bin" )
	end

	if ( opt.defs ) then defines( opt.defs ) end
	files { opt.files }

	kind "WindowedApp"

    if ( os.get() == "macosx" ) then
        -- C++11 libs
        linkoptions { "-stdlib=libc++" }

        if user.macports then
            -- Add Macports for dependencies
            includedirs { "/opt/local/include" }
            libdirs { "/opt/local/lib" }
        end
    end

    if ( user.includes ) then
        includedirs(user.includes)
    end
    if ( user.libs ) then
        libdirs(user.libs)
    end

	configuration( "Release" )
		targetname( opt.name .. "Sample" )
		links( opt.links )

	configuration "Debug"
		targetname( opt.name .. "Sample_D" )
		links( opt.linksd or opt.links )

end


