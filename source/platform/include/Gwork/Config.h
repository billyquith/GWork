/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONFIG_H
#define GWK_CONFIG_H

// GWK_COMPILE_DLL
//  - We're compiling the Gwork.dll (or .dylib etc)
#if defined (GWK_COMPILE_DLL)

#   ifdef _WIN32
#       if defined (__GNUC__)
#           define GWK_EXPORT __attribute__((dllexport))
#       else
#           define GWK_EXPORT __declspec(dllexport)
#       endif
#   endif

// GWK_COMPILE_STATIC
//  - We're compiling Gwork as a static library
#elif defined (GWK_COMPILE_STATIC)

#   define GWK_EXPORT

// GWK_DLL
// - We're including Gwork using the dll
#elif defined (GWK_DLL)

#   ifdef _WIN32
#       ifdef __GNUC__
#           define GWK_EXPORT __attribute__((dllimport))
#       else
#           define GWK_EXPORT __declspec(dllimport)
#       endif
#   endif

// - We're including Gwork using a static library
#else // if defined (GWK_COMPILE_DLL)

#   define GWK_EXPORT

#endif // if defined (GWK_COMPILE_DLL)

#ifndef GWK_EXPORT
#   define GWK_EXPORT
#endif


// Enable animation functions.
#define GWK_ANIMATE 1


#ifndef GWK_REFLECT
#   define GWK_REFLECT 0
#endif

#if GWK_REFLECT
#   include <ponder/pondertype.hpp>
#   define GWK_IF_REFLECT(SRC) SRC
#else
#   define GWK_IF_REFLECT(SRC) // ignore
#endif


#ifndef GWK_ALLOC_STATS
#   define GWK_ALLOC_STATS 0
#endif

#if GWK_ALLOC_STATS
#   include <memory>
    void* operator new(std::size_t size) throw(std::bad_alloc);
    void operator delete(void *mem) throw();
#   define GWK_IF_ALLOC_STATS(SRC) SRC
#else
#   define GWK_IF_ALLOC_STATS(SRC) // ignore
#endif // GWK_ALLOC_STATS

#endif // GWK_CONFIG_H

