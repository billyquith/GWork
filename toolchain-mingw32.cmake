# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-c++)
SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /opt/local/x86_64-w64-mingw32 /Users/sabetts/data-drive/work/mingw/SDL2-2.0.6/x86_64-w64-mingw32 /Users/sabetts/data-drive/work/mingw/SDL2_image-2.0.1/x86_64-w64-mingw32 /Users/sabetts/data-drive/work/mingw/SDL2_mixer-2.0.1/x86_64-w64-mingw32 /Users/sabetts/data-drive/work/mingw/SDL2_ttf-2.0.14/x86_64-w64-mingw32 /Users/sabetts/data-drive/work/mingw/iconv_x86_64 )

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

include_directories(/Users/sabetts/data-drive/work/mingw/iconv_x86_64/include)
