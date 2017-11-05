
Attic
-----

This directory is for old code that might be useful as a reference.

### Renderers

Direct2D and GDI+ were retired because I expect noone will ever use them. Even if I were
developing for one platform I'd still consider using a cross platform API, like Allegro5, BGFX,
or SDL2. Or a renderer, like Ogre or Irrlicht.
Related articles:

- [Direct2d vs GDI+](https://msdn.microsoft.com/en-us/library/windows/desktop/ff729480(v=vs.85).aspx).
- [Direct2D discussion](http://braid-game.com/news/2009/01/a-demonstration-that-direct2d-is-patently-ridiculous/).
