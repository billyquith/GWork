GWoRK
=====

An experimental *GWen foRK*. This is to fix some issues with GWEN and add some experimental new features.

*Note*: this is a *branch* from mainline. It is intended that mainline will still be pulled and elements
cherry picked. I embed the gwork branch in my project so the bias is on [how I like it][1]. I am mainly
interested in cross-platform development using [Allegro](http://alleg.sourceforge.net), developing on a Mac.

## Changes

### Features

* Cache to texture implemented for Allegro.
  * Controls render to a texture, which when nothing changing is a lot more efficient.
* No dependency on Bootil.
  * The experimental Designer had a dependency on Bootil. A new library in Gwen replaces the subset of code used.  
* *Fix warnings (WIP)*:
  * In GWEN, warnings are removed through pragmas. This is fine if you compile it standalone, but where you 
    include it in your own code, and you don't use pragmas, many warnings will show up.
  * Many warnings appear on compilers/platforms that aren't MSVC.
  * 64-bit warnings fixed. Gwen assumed 32-bit compilation.
* Fixes for Allegro.
  * Various fixes, e.g. text rendering. See Github issues and commits for more details.
  
### Format & Docs
  
* Doxygen docs:
  * Existing comments and docs formatted for [doxygen](http://doxygen.org).
  * Docs currently very minimal as Gwen has almost no documentation.
* Reformatted:
  * [Spaces instead of tabs](http://www.jwz.org/doc/tabs-vs-spaces.html).
  * Github uses 8 space tabs. GWEN uses 4 space tabs. This messes up the indentation when reading code
    on the Github.
  * [Allman][2]/Stroustrup indentation.
* Line length ~100 chars.
  * Github has width around 100. [Example][3].
  * Easier for 3 way merge. Everything on screen.
  * Works better for editing on laptop with a smaller screen.


[1]: http://www.codinghorror.com/blog/2009/04/death-to-the-space-infidels.html "Interesting article on consistency"
[2]: http://en.wikipedia.org/wiki/Indent_style#Allman_style "Not uncommon"
[3]: https://github.com/billyquith/GWEN/blob/gwork/gwen/include/Gwen/Skins/TexturedBase.h "Off the edge"


Thanks to Garry for GWEN.

BQ
