
local c_platform = os.get()
print("Platform: " .. c_platform)
dofile( c_platform .. ".lua" )


function DefineRenderer( name, filetable )

	project ( "Renderer-"..name )
	files( filetable )
	flags( { "Symbols" } )
	kind( "StaticLib" )

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

	configuration( "Release" )
		targetname( opt.name .. "Sample" )
		links( opt.links )

	configuration "Debug"
		targetname( opt.name .. "Sample_D" )
		links( opt.linksd or opt.links )

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

end


