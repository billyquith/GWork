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

        //! Calculate resource paths relative to the executable.
        class RelativeToExecutablePaths : public ResourcePaths
        {
            String m_resDir;
        public:
            //! Constructor
            //! \param resourceDir : optional directory, relative to the executable.
            RelativeToExecutablePaths(String const& resourceDir = String());
            String GetPath(Type type, String const& name) final;
        };

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
    
    namespace Log
    {
        enum class Level
        {
            Info,
            Warning,
            Error,
            Fatal
        };

        //! Write a message to the platform log.
        //! \param lvl : The level of the log.
        //! \param format : A message, using printf() style formatting.
        void Write(Level lvl, const char *format, ...);
        
        typedef void (*LogListener)(Level,const char*);
        
        //! Set where logs messages are sent to.
        //! \param listener : The new listener.
        //! \return The current log listener.
        LogListener SetLogListener(LogListener listener);
    }
    
    // Do not use this! Use Gwk::Log::Write()
    // Needs to be implemented by the platform.
    namespace Platform { void DefaultLogListener(Gwk::Log::Level lvl, const char *message); }
}

#endif // ifndef GWK_PLATFORM_COMMON_H
