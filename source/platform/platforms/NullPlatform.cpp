/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Platform.h>

static Gwk::String gs_ClipboardEmulator;

Gwk::String Gwk::Platform::GetExecutableDir()
{
    return String();
}

void Gwk::Platform::Sleep(unsigned int ms)
{
}

float Gwk::Platform::GetTimeInSeconds()
{
    return 0.f;
}

// Default place log messages are sent to.
void Gwk::Platform::DefaultLogListener(Gwk::Log::Level lvl, const char *message)
{
    if (lvl >= Gwk::Log::Level::Error)
        fputs(message, stderr);
    else
        fputs(message, stdout);
}

void Gwk::Platform::SetCursor(unsigned char iCursor)
{
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
