
Changelog
---------

This is a changelog for Gwork. There are also notes [on my blog][blog]

### 0.1

- Added a **Null render target** that doesn't render.
  - This makes CI testing easier as no graphical API dependencies.
  - Memory stats can be gathered more simply.
- Added **Travis continuous integration**.
  - This was hard work due to Travis' poor support for up-to-date compilers.
- Added **memory allocation stats** gathering.
  - Allocations are recorded and stats generated. Allows stats and memory leaks to be viewed.
  - Want to keep track of number & size of allocations. Hopefully this will be reduced in the future.
- Linux GCC compilers can use `iconv` instead of `codecvt` for Unicode conversion. `codecvt`
  is a C++11 feature which Apple and Microsoft support, but GCC was slow to adopt in libstdc++.


[blog]: http://chinbilly.blogspot.co.uk/search/label/gwork

