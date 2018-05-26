/*
 *  Gwork - CrossPlatform
 *
 *  - This is functionality that is implemented in a cross platform way. Some of the
 *    cross-platform APIs are treated as platforms and their APIs used instead.
 *
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Platform.h>

#ifdef _WIN32
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

static Gwk::String gs_ClipboardEmulator;


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

void Gwk::Platform::Sleep(unsigned int ms)
{
    // Only MSVC different.
#ifdef _WIN32
    ::Sleep(ms);
#else
    ::sleep(ms);
#endif
}

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

// Default place log messages are sent to.
void Gwk::Platform::DefaultLogListener(Gwk::Log::Level lvl, const char *message)
{
#if defined(_MSC_VER)
    OutputDebugString(message);
#else
    if (lvl >= Gwk::Log::Level::Error)
        fputs(message, stderr);
    else
        fputs(message, stdout);
#endif
}

void Gwk::Platform::SetCursor(unsigned char iCursor)
{
    // No platform independent way to do this
}

Gwk::String Gwk::Platform::GetClipboardText()
{
    return gs_ClipboardEmulator;
}

bool Gwk::Platform::SetClipboardText(const Gwk::String& str)
{
    gs_ClipboardEmulator = str;
    return true;
}

bool Gwk::Platform::FileOpen(const String& Name, const String& StartPath, const String& Extension,
                             String& filePathOut)
{
    return false;
}

bool Gwk::Platform::FileSave(const String& Name, const String& StartPath, const String& Extension,
                             String& filePathOut)
{
    return false;
}

bool Gwk::Platform::FolderOpen(const String& Name, const String& StartPath, String& filePathOut)
{
    return false;
}

void* Gwk::Platform::CreatePlatformWindow(int x, int y, int w, int h,
                                          const Gwk::String& strWindowTitle)
{
    return nullptr;
}

void Gwk::Platform::DestroyPlatformWindow(void* ptr)
{
}

bool Gwk::Platform::MessagePump(void* window)
{
    return false;
}

void Gwk::Platform::SetBoundsPlatformWindow(void* ptr, int x, int y, int w, int h)
{
}

void Gwk::Platform::SetWindowMaximized(void* ptr, bool bMax, Gwk::Point& newPos,
                                       Gwk::Point& newSize)
{
}

void Gwk::Platform::SetWindowMinimized(void* ptr, bool bMinimized)
{
}

bool Gwk::Platform::IsFocussedPlatformWindow(void* ptr)
{
    return true;
}

void Gwk::Platform::GetDesktopSize(int& w, int& h)
{
    w = 1024;
    h = 768;
}

void Gwk::Platform::GetCursorPos(Gwk::Point& po)
{
}
