GWork
=====

Linux & MacOS status: [![Build Status](https://travis-ci.org/billyquith/GWork.svg?branch=gwork)](https://travis-ci.org/billyquith/GWork)  Windows: [![Build status](https://ci.appveyor.com/api/projects/status/chlm41pnqchxlpv2/branch/gwork?svg=true)](https://ci.appveyor.com/project/billyquith/gwork/branch/gwork)

GWork is a skinnable, embeddable GUI library with an extensive control set. Control rendering
is abstracted, and can be implemented by any application wishing to use the library.
Gwork (*pronounced "gw-orc"*) is a fork of the GUI library [GWEN][gwen]. It was forked 
to fix issues with GWEN and add new features.

A number of rendering backends are provided for use, or as an example for your own:

 * [Allegro5][al5] (cross-platform).
 * OpenGL (cross-platform using GLFW).
 * [SDL2][sdl2] (cross-platform).
 * [SFML2][sfml2] (cross-platform).

## Documentation

- [Online documentation][docs]

## Changes

- See CHANGELOG for changes to Gwork, and the differences from GWEN.

## Issues

Please [report problems to Github][issues] or they'll get lost.

## Build

Get source code:

* From git: `git clone https://github.com/billyquith/GWork.git gwork` or
* [Download zip](https://github.com/billyquith/GWork/archive/gwork.zip) & unzip

CMake is used to generate the project files. See `cmake -h` to see all the generators for 
your platform. Only have one renderer per build directory. Choose renderer:

* `-DWANT_RENDERER_ALLEGRO5=ON`
* `-DWANT_RENDERER_OPENGL=ON`
* `-DWANT_RENDERER_SDL2=ON`
* `-DWANT_RENDERER_SFML2=ON`

For example to build Allegro 5 renderer using Ninja:

```bash
cd gwork
mkdir build && cd build                 # put build files in subdirectory
cmake -GNinja -DWANT_RENDERER_ALLEGRO5=ON ..   # create Ninja project files
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
[docs]: https://billyquith.github.io/GWork/
[issues]: https://github.com/billyquith/GWork/issues "Bugs/Issues"
