/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#ifndef GWK_MACROS_H
#define GWK_MACROS_H

// C RunTime Header Files
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <algorithm>

#ifdef _WIN32

#   ifndef NOMINMAX
#       define NOMINMAX
#   endif
#   include <windows.h>

#elif defined (__APPLE__)

#   include <CoreFoundation/CoreFoundation.h>

#elif defined (__linux__)

#else // ifdef _WIN32

#   error MUST_IMPLEMENT_PLATFORM

#endif // ifdef _WIN32

//! Get the number of items in a static array.
//! @param A : The array.
#define GWK_ARRAY_COUNT(A) (sizeof(A)/sizeof(A[0]))

namespace Gwk
{
    template <typename T>
    inline T Clamp(T current, T vmin, T vmax)
    {
        return current < vmin ? vmin : (current > vmax ? vmax : current);
    }

    template <typename T, typename T2>
    inline T Approach(T fCurrent, T fTarget, T2 fDelta)
    {
        if (fCurrent < fTarget)
        {
            fCurrent += fDelta;

            if (fCurrent > fTarget)
                return fTarget;
        }
        else if (fCurrent > fTarget)
        {
            fCurrent -= fDelta;

            if (fCurrent < fTarget)
                return fTarget;
        }

        return fCurrent;
    }

}

#endif // ifndef GWK_MACROS_H
