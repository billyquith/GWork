GWork
=====

GWork ("gw-orc") is a GWen fork to fix issues with GWEN and add new features.

*Note*: this is a *fork* from mainline, but not a rewrite. This was done to make
the source code more consistent with other C++ projects and [how I like it][1]. 
It is intended that mainline will still be cherry picked. I mainly develop using 
[Allegro](http://alleg.sourceforge.net) on a Mac, but everything else should 
still work. Changes summarised below.

Thanks to Garry for open sourcing GWEN.

## Changes

### Features

* [SDL2][8] renderer added.
  * *Tested on OSX and Windows*.
* [UTF-8 everywhere][5]. Unicode support simplified.
  * `Gwen::UnicodeString` removed. This assumed that all Unicode was best as a 
    wide encoding. This is not the case on all platforms.
  * `Gwen::TextObject` removed. This stored two copies of each string, one as 
    ASCII and one in wide Unicode. This is no longer necessary as all Unicode 
    is dealt with as UTF-8, which is backwards compatible with `std::string`.
* Cache to texture implemented (Allegro only).
  * Controls render to a texture, which when nothing changing is a lot more 
    efficient.
* No dependency on Bootil.
  * The experimental Designer had a dependency on Bootil. A new library in Gwen 
    replaces the subset of code used.  

### C++11

* C++11 is required:
  * locale is used for Unicode narrow/widen.
  
### Fixes

* Warnings fixed:
  * In GWEN, warnings are removed through pragmas. This is fine if you compile 
    it standalone, but where you include it in your own code, and you don't use 
    pragmas, many warnings will show up.
  * Many warnings appear on compilers/platforms that aren't MSVC.
  * 64-bit warnings fixed. GWEN assumed 32-bit compilation.
* Fixes for [Allegro][4].
  * Various fixes, e.g. text rendering. See Github issues and commits for more 
    details.
  
### Format
  
* Indentation:
  * [Spaces instead of tabs](http://www.jwz.org/doc/tabs-vs-spaces.html).
  * Github, Google Code, etc use 8 space tabs. GWEN uses 4 space tabs. This 
    messes up the indentation when reading code on Github.
* Brackets:
  * [Allman][2]/BSD indentation.
* Line length ~100 chars.
  * Github has width around 100. [Example][3].
  * Easier for 3 way merge. Everything on screen.
  * Works better for editing on laptop with a smaller screen.
  
### Docs

* Doxygen docs:
  * Any existing GWEN comments and docs formatted 
    for [doxygen](http://doxygen.org).
  * Docs currently very minimal as GWEN has almost no documentation.


## Build

[Premake][6] is used to generate the project files.

* Clone GWork.
* `cd gwen/Projects`
* Generate project with Premake:
  * MSVC: `premake vs2010`
  * Xcode: `premake-osx xcode4`
* Open the solution file and build sample-XXX for your chosen SDK.
  
Please [report problems to Github][7] or they'll get lost.


[1]: http://www.codinghorror.com/blog/2009/04/death-to-the-space-infidels.html "Interesting article on consistency"
[2]: http://en.wikipedia.org/wiki/Indent_style#Allman_style "Not uncommon"
[3]: https://github.com/billyquith/GWEN/blob/gwork/gwen/include/Gwen/Skins/TexturedBase.h "Off the edge"
[4]: http://alleg.sourceforge.net "Allegro 5"
[5]: http://www.utf8everywhere.org "Why you should use UTF8 everywhere."
[6]: http://industriousone.com/premake
[7]: https://github.com/billyquith/GWEN/issues "Bugs/Issues"
[8]: http://www.libsdl.org "SDL2"


BQ