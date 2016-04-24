GWork
=====

GWork is a skinnable, embeddable GUI library with an extensive control set. Control rendering
is abstracted, and can be implemented by any application wishing to use the library.
Gwork (*pronounced "gw-orc"*) is a fork of the GUI library [GWEN][gwen]. It was forked 
to fix issues with GWEN and add new features.

A number of rendering backends are provided for use, or as an example for your own:
 * [Allegro5][al5] (cross-platform).
 * TODO: Direct2D (Windows).
 * TODO: DirectX9 (Windows).
 * TODO: GDI+ (Windows).
 * [SDL2][sdl2] (cross-platform).
 * [SFML2][sfml2] (cross-platform).

## Build

Get source code:

* From git: `git clone https://github.com/billyquith/GWork.git gwork` or
* [Download zip](https://github.com/billyquith/GWork/archive/gwork.zip) & unzip

CMake is used to generate the project files.

```bash
cd gwork
mkdir build && cd build             # put all build files in separate directory
cmake -G Ninja -DRENDER_SDL2=ON ..  # create Ninja project files with SDL2 renderer
```

Providing the dependencies are present, this will create a sample executable. When run it will
demonstrate all of the controls available:

```bash
ninja                 # build project
bin/GworkSDL2Sample   # run sample
```

## Changes from GWEN

### Features

* CMake is used to generate project files instead of Premake. CMake is much more
  comprehensive.
* [SDL2][sdl2] renderer added.
* C++11 used.
* [UTF-8 everywhere][5]. Unicode support simplified.
  * `Gwen::UnicodeString` removed. This assumed that all Unicode was best as a 
    wide encoding. This is not the case on all platforms.
  * `Gwen::TextObject` removed. This stored two copies of each string, one as 
    ASCII and one in wide Unicode. This is no longer necessary as all Unicode 
    is dealt with as UTF-8, which is backwards compatible with `std::string`.
* Documentation:
  * Any existing GWEN comments and docs formatted for [doxygen](http://doxygen.org).
  * Docs currently very minimal as GWEN has almost no documentation.
* Cache to texture optimisation implemented (Allegro only).
* No dependency on Bootil (Garry's personal library).
* Fixes for [Allegro][al5]. e.g. text rendering.
* Many warnings fixed.

### Source code formatting
  
* The source code has been reformated to be more consistent with other C++ 
  projects and [how I like it][1].
* Indentation: [Spaces instead of tabs](http://www.jwz.org/doc/tabs-vs-spaces.html).
  Github, Google Code, etc use 8 space tabs. GWEN uses 4 space tabs. This 
  messes up the indentation when reading code on Github.
* Brackets: [Allman][2]/BSD indentation.
* Line length ~100 chars. Github has width around 100. Easier for 
  3 way merge. Everything on regular screen.
* camelCase variables.
  
Please [report problems to Github][7] or they'll get lost.


[gwen]: https://github.com/garrynewman/GWEN
[sdl2]: https://www.libsdl.org/
[sfml2]: http://www.sfml-dev.org
[al5]: http://alleg.sourceforge.net
[1]: http://www.codinghorror.com/blog/2009/04/death-to-the-space-infidels.html "Interesting article on consistency"
[2]: http://en.wikipedia.org/wiki/Indent_style#Allman_style "Not uncommon"
[5]: http://www.utf8everywhere.org "Why you should use UTF8 everywhere."
[6]: http://industriousone.com/premake
[7]: https://github.com/billyquith/GWork/issues "Bugs/Issues"


BQ
