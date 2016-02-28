/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_EXPORTS_H
#define GWK_EXPORTS_H

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
//  - We're compiling gwen as a static library
#elif defined (GWK_COMPILE_STATIC)

#   define GWK_EXPORT

// GWK_DLL
// - We're including gwen using the dll
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

#endif // GWK_EXPORTS_H
