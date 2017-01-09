
Changelog
---------

This is a changelog for Gwork. There are also notes [on my blog][blog]

### 0.2

- `override` used everywhere applicable to marked overridden virtual functions.
- Online [documentation][docs] added.

### 0.1

- CMake is used to generate project files instead of Premake. CMake is much more
  comprehensive.
- C++11 used.
- [SDL2][sdl2] renderer added.
- Cache to texture optimisation implemented (Allegro only).
- Added a **Null render target** that doesn't render.
  - This makes CI testing easier as no graphical API dependencies.
  - Memory stats can be gathered more simply.
- [UTF-8 everywhere][5]. Unicode support simplified.
  - `Gwen::UnicodeString` removed. This assumed that all Unicode was best as a 
    wide encoding. This is not the case on all platforms.
  - `Gwen::TextObject` removed. This stored two copies of each string, one as 
    ASCII and one in wide Unicode. This is no longer necessary as all Unicode 
    is dealt with as UTF-8, which is backwards compatible with `std::string`.
- Added **Travis continuous integration**.
  - This was hard work due to Travis' poor support for up-to-date compilers.
- Added **memory allocation stats** gathering.
  - Allocations are recorded and stats generated. Allows stats and memory leaks to be viewed.
  - Want to keep track of number & size of allocations. Hopefully this will be reduced in the future.
- Documentation:
  - Any existing GWEN comments and docs formatted for [doxygen](http://doxygen.org).
  - Docs currently very minimal as GWEN has almost no documentation.
- Linux GCC compilers can use `iconv` instead of `codecvt` for Unicode conversion. `codecvt`
  is a C++11 feature which Apple and Microsoft support, but GCC was slow to adopt in libstdc++.
- No dependency on Bootil (Garry's personal library).
- Fixes for [Allegro][al5]. e.g. text rendering.
- Many warnings fixed.

### Source code formatting
  
* The source code has been reformated to be more consistent with other C++ 
  projects and [how I like it][1].
* Indentation: [Spaces instead of tabs](http://www.jwz.org/doc/tabs-vs-spaces.html).
  Github, Google Code, etc use 8 space tabs. GWEN uses 4 space tabs. This 
  messes up the indentation when reading code on Github.
* Brackets: [Allman][2]/BSD indentation.
* Line length ~100 chars. Github has width around 100. Easier for 3 way merge. Everything 
  on regular screen.
* camelCase variables.
  

[gwen]: https://github.com/garrynewman/GWEN
[sdl2]: https://www.libsdl.org/
[sfml2]: http://www.sfml-dev.org
[al5]: http://alleg.sourceforge.net
[issues]: https://github.com/billyquith/GWork/issues "Bugs/Issues"
[1]: http://www.codinghorror.com/blog/2009/04/death-to-the-space-infidels.html "Interesting article on consistency"
[2]: http://en.wikipedia.org/wiki/Indent_style#Allman_style "Not uncommon"
[5]: http://www.utf8everywhere.org "Why you should use UTF8 everywhere."
[6]: http://industriousone.com/premake
[blog]: http://chinbilly.blogspot.co.uk/search/label/gwork
[docs]: https://billyquith.github.io/GWork/

