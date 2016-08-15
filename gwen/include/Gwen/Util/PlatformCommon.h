/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_PLATFORM_COMMON_H
#define GWK_PLATFORM_COMMON_H

#include <Gwen/Exports.h>
#include <vector>

namespace Gwk
{
    namespace Platform
    {
        // Low level functionality shared by all of the platforms.

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
