/*
 *  Gwork
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/PlatformCommon.h>
#include <Gwork/Version.h>

#ifdef _MSC_VER
#   define WIN32_LEAN_AND_MEAN 
#   include <Windows.h>
#   include <WinBase.h>     // sleep
#   undef min
#   undef max
#else
#   include <unistd.h>
#endif
#include <time.h>

#if defined(__APPLE__)
#   include <errno.h>
#   include <libgen.h>
#   include <libproc.h>
#endif

#include <cstdlib>
#include <map>
#include <iostream>

using namespace Gwk;

//------------------------------------------------------------------------------
// Platform functions, implemented in a cross-platform way. This is to
// avoid duplicating the functionality in the platform-specific files.

// Only the following platforms require these functions:
#if defined(GWK_PLATFORM_Null)

// GetExecutableDir()
//-------------------

Gwk::String Gwk::Platform::GetExecutableDir()
{
#if defined(__APPLE__)

    // OSX: Use process information.

    const pid_t pid = getpid();
    char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
    const int ret = proc_pidpath(pid, pathbuf, sizeof(pathbuf));
    if (ret > 0)
    {
        return String(dirname(pathbuf)) + "/";
    }

    // fprintf(stderr, "PID %d: %s\n", pid, strerror(errno));
    return "";

#elif defined(WIN32)

    // Windows: Use the module information.

    char path[MAX_PATH] = { '\0' };
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL)
    {
        GetModuleFileName(hModule, path, sizeof(path));

        if (path)
        {
            // get directory name
            char *dir = strrchr(path, '\\');
            if (dir)
                dir[1] = '\0';
        }
    }
    return String(path);

#else
    
    return String();
    
#endif
}

// Sleep()
//--------

void Gwk::Platform::Sleep(unsigned int ms)
{
#ifdef _MSC_VER
    ::Sleep(ms);
#else
    ::sleep(ms);
#endif
}

// GetTimeInSeconds()
//-------------------

#ifdef WIN32

static double GetPerformanceFrequency()
{
    static double Frequency = 0.0f;

    if (Frequency == 0.0f)
    {
        __int64 perfFreq;
        QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
        Frequency = 1.0 / perfFreq;
    }

    return Frequency;
}

float Gwk::Platform::GetTimeInSeconds()
{
    static float fCurrentTime = 0.0f;
    static __int64 iLastTime = 0;
    __int64 thistime;

    QueryPerformanceCounter((LARGE_INTEGER*)&thistime);
    double fSecondsDifference = (thistime - iLastTime)*GetPerformanceFrequency();

    if (fSecondsDifference > 0.1)
        fSecondsDifference = 0.1;

    fCurrentTime += fSecondsDifference;
    iLastTime = thistime;
    return fCurrentTime;
}

#else

float Gwk::Platform::GetTimeInSeconds()
{
    const float seconds = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    return seconds;
}

#endif 

#endif // defined(GWK_PLAFORM_Null)

//------------------------------------------------------------------------------

Gwk::Platform::RelativeToExecutablePaths::RelativeToExecutablePaths(String const& resourceDir)
:   m_resDir(GetExecutableDir() + resourceDir)
{}

String Gwk::Platform::RelativeToExecutablePaths::GetPath(Type type, String const& filename)
{
    return m_resDir + filename;
}

//==============================================================================

#if GWK_ALLOC_STATS

static Platform::AllocStats g_stats;

const Platform::AllocStats& Platform::GetAllocStats()
{
    return g_stats;
}

//------------------------------------------------------------------------------

namespace {

// Based on: http://stackoverflow.com/a/439876/3233

// Custom allocator avoiding new
template<typename T>
struct MallocAllocator : std::allocator<T>
{
    typedef typename std::allocator<T>::pointer pointer;
    typedef typename std::allocator<T>::size_type size_type;
    
    template<typename U>
    struct rebind {
        typedef MallocAllocator<U> other;
    };
    
    MallocAllocator() {}
    
    template<typename U>
    MallocAllocator(MallocAllocator<U> const& u)
    :   std::allocator<T>(u) {}
    
    pointer allocate(size_type size, std::allocator<void>::const_pointer = 0)
    {
        void * p = std::malloc(size * sizeof(T));
        if(p == 0)
            throw std::bad_alloc();
        
        return static_cast<pointer>(p);
    }
    
    void deallocate(pointer p, size_type)
    {
        std::free(p);
    }
};

typedef std::map< void*, std::size_t, std::less<void*>,
                  MallocAllocator< std::pair<void* const, std::size_t> > > TrackerMemMap;

struct AllocReporter
{
    TrackerMemMap *track;
    
    AllocReporter(TrackerMemMap *track) : track(track) {}
    
    ~AllocReporter()
    {
        TrackerMemMap::const_iterator it = track->begin();
        while (it != track->end())
        {
            std::cerr << "TRACK: leaked at " << it->first << ", " << it->second << " bytes\n";
            ++it;
        }
    }
};

static TrackerMemMap* GetAllocTracker()
{
    // don't use normal new to avoid infinite recursion.
    static TrackerMemMap *track = new (std::malloc(sizeof *track)) TrackerMemMap;
    static AllocReporter printer(track);
    return track;
}

struct Mark
{
    const char * const name;
    Platform::AllocStats stats;
};

typedef std::vector<Mark, MallocAllocator<Mark>> MarksType;
MarksType g_marks;
    
} // namespace {

//----------------------------------------------------------------------------------

void Platform::AllocStatsAddMark(const char * const name)
{
    Mark m = { name, Platform::GetAllocStats() };
    g_marks.emplace_back(m);;
}

void Platform::AllocStatsDump(FILE *fh)
{
    fprintf(fh, "Mark,CumNumAllocs,CumAllocSize,CurrNumAllocs,CurrAllocSize\n");
    for (auto&& m : g_marks)
    {
        fprintf(fh, "%s,%ld,%ld,%ld,%ld\n", m.name,
                m.stats.cumulativeNumAllocs, m.stats.cumulativeAllocBytes,
                m.stats.currentNumAllocs, m.stats.currentAllocBytes);
    }
}

//------------------------------------------------------------------------------

// These are declared in Config.h, which should be included everywhere.

void* operator new(std::size_t size) throw(std::bad_alloc)
{
    // we are required to return non-null
    void *mem = std::malloc(size == 0 ? 1 : size);
    if (mem == 0)
        throw std::bad_alloc();
    
    ++g_stats.currentNumAllocs;
    ++g_stats.cumulativeNumAllocs;
    g_stats.currentAllocBytes += size;
    g_stats.cumulativeAllocBytes += size;
    (*GetAllocTracker())[mem] = size;
    
    return mem;
}

void operator delete(void *mem) throw()
{
    TrackerMemMap *trk = GetAllocTracker();
    auto it = trk->find(mem);
    
    if (it != trk->end())
    {
        const std::size_t size = it->second;
        
        --g_stats.currentNumAllocs;
        g_stats.currentAllocBytes -= size;
        
        trk->erase(it);
    }
    else
    {
        // this indicates a serious bug
        std::cerr << "bug: memory at " << mem << " wasn't allocated by us\n";
    }
        
    std::free(mem);
}

#endif // GWK_ALLOC_STATS

