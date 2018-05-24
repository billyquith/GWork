GWork
=====

Linux & MacOS status: [![Build Status](https://travis-ci.org/billyquith/GWork.svg?branch=gwork)](https://travis-ci.org/billyquith/GWork)  Windows: [![Build status](https://ci.appveyor.com/api/projects/status/chlm41pnqchxlpv2/branch/gwork?svg=true)](https://ci.appveyor.com/project/billyquith/gwork/branch/gwork)

GWork is a skinnable, embeddable GUI library with an extensive control set. Control rendering
is abstracted, and can be implemented by any application wishing to use the library.
Gwork (*pronounced "gw-orc"*) is a fork of the GUI library [GWEN][gwen]. It was forked
to fix issues with GWEN and add new features.

A number of rendering backends are provided for use, or as an example for your own:

 * [Allegro5][al5] (cross-platform).
 * DirectX 11 (Windows 7+).
 * [Irrlicht3D][irr3d] (cross-platform).
 * OpenGL2 (cross-platform. Uses GLFW).
 * OpenGL Core Profile (cross-platform. Uses GLM, GLEW, GLFW).
 * [SDL2][sdl2] (cross-platform).
 * [SFML2][sfml2] (cross-platform).
 * Software (cross-platform). Render to texture.
 
Note that the software renderer can be used on any platform, but, obviously, with the penalty of 
not having hardware acceleration. For more information see the documentation.

## Documentation

- [Online documentation][docs]

## Changes

- See [CHANGELOG][changes] for changes to Gwork, and the differences from GWEN.

## Issues

Please [report problems to Github][issues] or they'll get lost.

## Build

Get source code:

* From git: `git clone https://github.com/billyquith/GWork.git gwork` or
* [Download zip](https://github.com/billyquith/GWork/archive/gwork.zip) & unzip

CMake is used to generate the project files. See `cmake -h` to see all the generators for
your platform. Only have one renderer per build directory. Choose renderer:

* `-DRENDER_ALLEGRO5=ON`
* `-DRENDER_DIRECTX11=ON`
* `-DRENDER_IRRLICHT=ON`
* `-DRENDER_OPENGL=ON`
* `-DRENDER_OPENGL_CORE=ON`
* `-DRENDER_SDL2=ON`
* `-DRENDER_SFML2=ON`
* `-DRENDER_SW=ON`

For example to build Allegro 5 renderer using Ninja:

```bash
cd gwork
mkdir build && cd build                 # put build files in subdirectory
cmake -GNinja -DRENDER_ALLEGRO5=ON ..   # create Ninja project files
```

Providing the dependencies are present, this will create a sample executable. When run it will
demonstrate all of the controls available:

```bash
ninja                       # build project
bin/GworkAllegro5Sample     # run sample
```

[gwen]: https://github.com/garrynewman/GWEN
[sdl2]: https://www.libsdl.org/
[sfml2]: http://www.sfml-dev.org
[al5]: http://alleg.sourceforge.net
[irr3d]: http://irrlicht.sourceforge.net/
[docs]: https://billyquith.github.io/GWork/
[changes]: https://github.com/billyquith/GWork/blob/gwork/CHANGELOG.md
[issues]: https://github.com/billyquith/GWork/issues "Bugs/Issues"
