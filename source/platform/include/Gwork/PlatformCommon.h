/*  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */
/*! \file PlatformCommon.h */


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
        //! I.e. path = exe_dir + resource_dir + relative_path.
        class RelativeToExecutablePaths : public ResourcePaths
        {
            String m_resDir;
        public:
            //! Constructor
            //! \param resourceDir : Optional directory, relative to the executable.
            RelativeToExecutablePaths(String const& resourceDir = String());
            
            //! Get full path to file.
            //! \param type : File resource type.
            //! \param relPath : Relative path to file.
            //! \return Path relative to the resource directory.
            String GetPath(Type type, String const& relPath) final;
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
        enum class Level    //!< Logging classification.
        {
            Info,       //!< Informational.
            Warning,    //!< A warning.
            Error,      //!< An error.
            Fatal       //!< Whoa! Fatal error!
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

    namespace Debug
    {
        //! \def GWK_ASSERT(TEST)
        //! Assertion check.
        //! If fails, application aborts.
        //! \param TEST : The Boolean assertion check.
#define GWK_ASSERT(TEST) if (!(TEST)) Gwk::Debug::AssertFail(#TEST);
        
        //! \def GWK_ASSERT_MSG(TEST, MESSAGE)
        //! Assertion check.
        //! If fails, application logs message and aborts.
        //! \param TEST : The Boolean assertion check.
        //! \param MESSAGE : The message to output on failure.
#define GWK_ASSERT_MSG(TEST, MESSAGE) if (!(TEST)) Gwk::Debug::AssertFail(MESSAGE)        

        //! Called when an assertion fails.
        GWK_EXPORT void AssertFail(const char* message);
    }
    
    // Do not use this! Use Gwk::Log::Write()
    // Needs to be implemented by the platform.
    namespace Platform { void DefaultLogListener(Gwk::Log::Level lvl, const char *message); }
}

#endif // ifndef GWK_PLATFORM_COMMON_H
