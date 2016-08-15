/*
 *  Gwork
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/PlatformCommon.h>

#include <cstdlib>
#include <map>
#include <iostream>


static Gwk::Platform::AllocStats g_stats;
static Gwk::Platform::MemoryReporter g_memrep;

const Gwk::Platform::AllocStats& Gwk::Platform::GetAllocStats()
{
    return g_stats;
}

Gwk::Platform::MemoryReporter& Gwk::Platform::GetAllocReporter()
{
    return g_memrep;
}

void Gwk::Platform::MemoryReporter::DumpStats()
{
    FILE *fh = stdout;
    fprintf(fh, "Mark,CurrNumAllocs,CurrAllocSize,NumAllocDiff,SizeAllocDiff\n");
    Platform::AllocStats lastStat;
    for (auto&& m : m_marks)
    {
        auto diff = m.stats;
        diff.currentNumAllocs -= lastStat.currentNumAllocs;
        diff.currentAllocBytes -= lastStat.currentAllocBytes;
        fprintf(fh, "%s,%ld,%ld,%ld,%ld\n", m.name.c_str(),
                m.stats.currentNumAllocs, m.stats.cumulativeAllocBytes,
                diff.currentNumAllocs, diff.currentAllocBytes);
        lastStat = m.stats;
    }
}

#if GWK_MEMORY_STATS

namespace {

// Based on: http://stackoverflow.com/a/439876/3233

// Custom allocator avoiding new
template<typename T>
struct AllocTracker : std::allocator<T>
{
    typedef typename std::allocator<T>::pointer pointer;
    typedef typename std::allocator<T>::size_type size_type;
    
    template<typename U>
    struct rebind {
        typedef AllocTracker<U> other;
    };
    
    AllocTracker() {}
    
    template<typename U>
    AllocTracker(AllocTracker<U> const& u)
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
                  AllocTracker< std::pair<void* const, std::size_t> > > TrackerMemMap;

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

} // namespace {

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


#endif // GWK_MEMORY_STATS

