GWoRK
=====

An experimental *GWen foRK*. This is to fix some issues with GWEN and add some experimental new features.

*Note*: this is a *branch* from mainline. It is intended that mainline will still be pulled and elements
cherry picked. I embed the gwork branch in my project so the bias is on [how I like it][1]. I am mainly
interested in cross-platform development using [Allegro](http://alleg.sourceforge.net), developing on a Mac.

### Changes

* *Fix warnings (WIP)*:
  * In GWEN, warnings are removed through pragmas. This is fine if you compile it standalone, but where you 
    include it in your own code, and you don't use pragmas, many warnings will show up.
  * Many warnings appear on compilers/platforms that aren't MSVC.
* Fixes for Allegro.
  * Various fixes, e.g. text rendering. See Github issues and commits for more details.
* Doxygen docs:
  * Existing comments and docs formatted for [doxygen](http://doxygen.org).
  * Docs very minimal. Helps with navigating the code to grok it.
* Reformatted:
  * [Spaces instead of tabs](http://www.jwz.org/doc/tabs-vs-spaces.html).
  * Github uses 8 space tabs. GWEN uses 4 space tabs. This messes up the indentation when reading code
    on the site, which I do quite a lot.
  * [Allman][2]/"ANSI" indentation.
* Line length ~100 chars.
  * Github has width around 100. [Example][3].
  * If you do diff or 3 way merge you can't all text if >100 chars.
  * Works better for editing on laptop (13" screen).


[1]: http://www.codinghorror.com/blog/2009/04/death-to-the-space-infidels.html "Interesting article on consistency"
[2]: http://en.wikipedia.org/wiki/Indent_style#Allman_style "Not uncommon"
[3]: https://github.com/billyquith/GWEN/blob/gwork/gwen/include/Gwen/Skins/TexturedBase.h "Off the edge"


Thanks to Garry for GWEN.

BQ
