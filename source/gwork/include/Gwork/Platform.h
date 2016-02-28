/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_PLATFORM_H
#define GWK_PLATFORM_H

#include "Gwork/Structures.h"
#include "Gwork/Events.h"

// Decide which platform to use.
//
// Note that cross-platform APIs, like Allegro and SDL, are also supported. We don't want to
// include their paths etc in the Gwork library, which should be library agnostic. We have a
// define, GWK_NOT_NATIVE_PLATFORM, which if defined means that we expect to link to these
// functions outside of the Gwork lib. Some basic implementations of platform functions are
// provided to fall back on should this not be defined.
//
//  - This platform bit is a bit inconsistent. The rest of Gwork is abstracted through
//    the Renderers, as Gwork is supposed to be embedded. Creating platform windows should probably
//    be done elsewhere. --BQ
//

// Hardcode for now.
//#ifdef __APPLE__
//#   define GWK_NOT_NATIVE_PLATFORM     // We don't have native support for this.
//#endif

#if defined(GWK_NOT_NATIVE_PLATFORM)
#   define GWK_PLATFORM_EXTERNAL       // Platform implemented outside Gwork.
#elif defined(_WIN32)
#   define GWK_PLATFORM_WINDOWS        // Use Windows API.
#else
#   define GWK_PLATFORM_NULL           // Most features not implemented, but does link.
#endif


namespace Gwk
{
    namespace Platform
    {
        //! Go to sleep for a time. Stops CPU hogging.
        //! @param iMS : Number of milliseconds to sleep for.
        GWK_EXPORT void Sleep(unsigned int iMS);

        // Needed for things like double click
        GWK_EXPORT float GetTimeInSeconds();

        //! Set the system cursor.
        //! @param cursor : Cursor number to use. @see CursorType
        GWK_EXPORT void SetCursor(unsigned char cursor);

        GWK_EXPORT void GetCursorPos(Gwk::Point& p);
        GWK_EXPORT void GetDesktopSize(int& w, int& h);

        //! Get platform clipboard as a string.
        GWK_EXPORT String GetClipboardText();
        //! Set platform clipboard from a string.
        GWK_EXPORT bool   SetClipboardText(const String& str);

        // System Dialogs ( Can return false if unhandled )
        //
        GWK_EXPORT bool FileOpen(const String& Name, const String& StartPath,
                                  const String& Extension, Gwk::Event::Handler* pHandler,
                                  Event::Handler::FunctionWithInformation fnCallback);
        GWK_EXPORT bool FileSave(const String& Name, const String& StartPath,
                                  const String& Extension, Gwk::Event::Handler* pHandler,
                                  Event::Handler::FunctionWithInformation fnCallback);
        GWK_EXPORT bool FolderOpen(const String& Name, const String& StartPath,
                                    Gwk::Event::Handler* pHandler,
                                    Event::Handler::FunctionWithInformation fnCallback);

        //
        // Window Creation
        //
        GWK_EXPORT void* CreatePlatformWindow(int x, int y, int w, int h,
                                               const Gwk::String& strWindowTitle);
        GWK_EXPORT void DestroyPlatformWindow(void* pPtr);
        GWK_EXPORT void SetBoundsPlatformWindow(void* pPtr, int x, int y, int w, int h);
        GWK_EXPORT void MessagePump(void* pWindow, Gwk::Controls::Canvas* ptarget);
        GWK_EXPORT bool HasFocusPlatformWindow(void* pPtr);
        GWK_EXPORT void SetWindowMaximized(void* pPtr, bool bMaximized, Gwk::Point& pNewPos,
                                            Gwk::Point& pNewSize);
        GWK_EXPORT void SetWindowMinimized(void* pPtr, bool bMinimized);
    }

}

#endif // ifndef GWK_PLATFORM_H
