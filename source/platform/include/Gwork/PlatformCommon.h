/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_PLATFORM_COMMON_H
#define GWK_PLATFORM_COMMON_H

#include <Gwork/PlatformTypes.h>
#include <vector>

namespace Gwk
{
    namespace Platform
    {
        // Cross-platform, low level functionality shared by all of the platforms.

        //! Go to sleep for a time. Stops CPU hogging.
        //! @param ms : Number of milliseconds to sleep for.
        GWK_EXPORT void Sleep(unsigned int ms);
        
        // Needed for things like double click
        GWK_EXPORT float GetTimeInSeconds();

        //! Get the directory of the running executable.
        //! \return Path string, blank if not known.
        //! \note Path is terminated with directory separator.
        GWK_EXPORT String GetExecutableDir();

#if GWK_ALLOC_STATS

        struct AllocStats
        {
            std::size_t currentAllocBytes = 0;
            std::size_t currentNumAllocs = 0;
            std::size_t cumulativeAllocBytes = 0;
            std::size_t cumulativeNumAllocs = 0;
        };
        
        //! Get the current allocation stats. Only active if GWK_ALLOC_STATS on.
        const AllocStats& GetAllocStats();
        
        void AllocStatsAddMark(const char * const name);
        void AllocStatsDump(FILE *fh = stdout);
        
#endif // GWK_ALLOC_STATS

    }
}

#endif // ifndef GWK_PLATFORM_COMMON_H
