/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

//#include "Gwork/Macros.h"
#include "Gwork/Platform.h"

#ifdef GWK_PLATFORM_NULL

#include <time.h>

static Gwk::String gs_ClipboardEmulator;

void Gwk::Platform::Sleep(unsigned int iMS)
{
    // TODO.
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

float Gwk::Platform::GetTimeInSeconds()
{
    float fSeconds = (float)clock()/(float)CLOCKS_PER_SEC;
    return fSeconds;
}

bool Gwk::Platform::FileOpen(const String& Name, const String& StartPath, const String& Extension,
                              Gwk::Event::Handler* pHandler,
                              Event::Handler::FunctionWithInformation fnCallback)
{
    // No platform independent way to do this.
    // Ideally you would open a system dialog here
    return false;
}

bool Gwk::Platform::FileSave(const String& Name, const String& StartPath, const String& Extension,
                              Gwk::Event::Handler* pHandler,
                              Gwk::Event::Handler::FunctionWithInformation fnCallback)
{
    // No platform independent way to do this.
    // Ideally you would open a system dialog here
    return false;
}

bool Gwk::Platform::FolderOpen(const String& Name, const String& StartPath,
                                Gwk::Event::Handler* pHandler,
                                Event::Handler::FunctionWithInformation fnCallback)
{
    return false;
}

void* Gwk::Platform::CreatePlatformWindow(int x, int y, int w, int h,
                                           const Gwk::String& strWindowTitle)
{
    return NULL;
}

void Gwk::Platform::DestroyPlatformWindow(void* pPtr)
{
}

void Gwk::Platform::MessagePump(void* pWindow, Gwk::Controls::Canvas* ptarget)
{
}

void Gwk::Platform::SetBoundsPlatformWindow(void* pPtr, int x, int y, int w, int h)
{
}

void Gwk::Platform::SetWindowMaximized(void* pPtr, bool bMax, Gwk::Point& pNewPos,
                                        Gwk::Point& pNewSize)
{
}

void Gwk::Platform::SetWindowMinimized(void* pPtr, bool bMinimized)
{
}

bool Gwk::Platform::HasFocusPlatformWindow(void* pPtr)
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

#endif // GWK_PLATFORM_NULL
