
#ifndef GWEN_MACROS_H
#define GWEN_MACROS_H

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


namespace Gwen
{
    template <typename T>
    inline T Min(T a, T b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    inline T Max(T a, T b)
    {
        return a > b ? a : b;
    }

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

#endif // ifndef GWEN_MACROS_H
