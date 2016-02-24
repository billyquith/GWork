GWork
=====

GWork (pronounced "gw-orc") is a fork of the GUI library [GWEN][gwen]. It was forked 
to fix issues with GWEN and add new features. 

## Build

CMake is used to generate the project files. A number of rendering backends are
provided:
 * [Allegro5][al5] (cross-platform).
 * Direct2D (Windows).
 * DirectX9 (Windows).
 * GDI+ (Windows).
 * [SDL2][sdl2] (cross-platform).
 * [SFML2][sfml2] (cross-platform).

```bash
git clone https://github.com/billyquith/GWork.git gwork
cd gwork
cmake -G Ninja -DRENDER_SDL2  # create Ninja project files with SDL2 renderer
ninja
```

## Changes from GWEN

### Features

* [SDL2][sdl2] renderer added. Tested on *OSX and Windows*.
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
* C++11 used, e.g. *locale* is used for Unicode narrow/widen.
* Fixes for [Allegro][al5]. e.g. text rendering.
* Many warnings fixed.

### Source code formatting
  
* The source code has been reformated to be more consistent with other C++ 
  projects and [how I like it][1].
* Indentation: [Spaces instead of tabs](http://www.jwz.org/doc/tabs-vs-spaces.html).
  Github, Google Code, etc use 8 space tabs. GWEN uses 4 space tabs. This 
  messes up the indentation when reading code on Github.
* Brackets: [Allman][2]/BSD indentation.
* Line length ~100 chars. Github has width around 100. [Example][3]. Easier for 
  3 way merge. Everything on screen. Works better for editing on laptop with 
  a smaller screen.
  
Please [report problems to Github][7] or they'll get lost.


[gwen]: https://github.com/garrynewman/GWEN
[sdl2]: https://www.libsdl.org/
[sfml2]: http://www.sfml-dev.org
[al5]: http://alleg.sourceforge.net
[1]: http://www.codinghorror.com/blog/2009/04/death-to-the-space-infidels.html "Interesting article on consistency"
[2]: http://en.wikipedia.org/wiki/Indent_style#Allman_style "Not uncommon"
[3]: https://github.com/billyquith/GWEN/blob/gwork/gwen/include/Gwen/Skins/TexturedBase.h "Off the edge"
[5]: http://www.utf8everywhere.org "Why you should use UTF8 everywhere."
[6]: http://industriousone.com/premake
[7]: https://github.com/billyquith/GWork/issues "Bugs/Issues"


BQ