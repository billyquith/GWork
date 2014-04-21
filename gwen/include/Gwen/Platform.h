/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#pragma once
#ifndef GWEN_PLATFORM_H
#define GWEN_PLATFORM_H

#include "Gwen/Structures.h"
#include "Gwen/Events.h"

// Decide which platform to use.
//
// Note that cross-platform APIs, like Allegro and SDL, are also supported. We don't want to
// include their paths etc in the GWEN library, which should be library agnostic. We have a
// define, GWEN_NOT_NATIVE_PLATFORM, which if defined means that we expect to link to these
// functions outside of the GWEN lib. Some basic implementations of platform functions are
// provided to fall back on should this not be defined.
//
//  - This platform bit is a bit inconsistent. The rest of GWEN is abstracted through
//    the Renderers, as GWEN is supposed to be embedded. Creating platform windows should probably
//    be done elsewhere. --BQ
//

// Hardcode for now.
#ifdef __APPLE__
#   define GWEN_NOT_NATIVE_PLATFORM     // We don't have native support for this.
#endif

#if defined(GWEN_NOT_NATIVE_PLATFORM)
#   define GWEN_PLATFORM_EXTERNAL       // Platform implemented outside Gwork.
#elif defined(_WIN32)
#   define GWEN_PLATFORM_WINDOWS        // Use Windows API.
#else
#   define GWEN_PLATFORM_NULL           // Most features not implemented, but does link.
#endif


namespace Gwen
{
    namespace Platform
    {
        //! Go to sleep for a time. Stops CPU hogging.
        //! @param iMS : Number of milliseconds to sleep for.
        GWEN_EXPORT void Sleep(unsigned int iMS);

        // Needed for things like double click
        GWEN_EXPORT float GetTimeInSeconds();

        //! Set the system cursor.
        //! @param cursor : Cursor number to use. @see CursorType
        GWEN_EXPORT void SetCursor(unsigned char cursor);

        GWEN_EXPORT void GetCursorPos(Gwen::Point& p);
        GWEN_EXPORT void GetDesktopSize(int& w, int& h);

        //! Get platform clipboard as a string.
        GWEN_EXPORT String GetClipboardText();
        //! Set platform clipboard from a string.
        GWEN_EXPORT bool   SetClipboardText(const String& str);

        // System Dialogs ( Can return false if unhandled )
        //
        GWEN_EXPORT bool FileOpen(const String& Name, const String& StartPath,
                                  const String& Extension, Gwen::Event::Handler* pHandler,
                                  Event::Handler::FunctionWithInformation fnCallback);
        GWEN_EXPORT bool FileSave(const String& Name, const String& StartPath,
                                  const String& Extension, Gwen::Event::Handler* pHandler,
                                  Event::Handler::FunctionWithInformation fnCallback);
        GWEN_EXPORT bool FolderOpen(const String& Name, const String& StartPath,
                                    Gwen::Event::Handler* pHandler,
                                    Event::Handler::FunctionWithInformation fnCallback);

        //
        // Window Creation
        //
        GWEN_EXPORT void* CreatePlatformWindow(int x, int y, int w, int h,
                                               const Gwen::String& strWindowTitle);
        GWEN_EXPORT void DestroyPlatformWindow(void* pPtr);
        GWEN_EXPORT void SetBoundsPlatformWindow(void* pPtr, int x, int y, int w, int h);
        GWEN_EXPORT void MessagePump(void* pWindow, Gwen::Controls::Canvas* ptarget);
        GWEN_EXPORT bool HasFocusPlatformWindow(void* pPtr);
        GWEN_EXPORT void SetWindowMaximized(void* pPtr, bool bMaximized, Gwen::Point& pNewPos,
                                            Gwen::Point& pNewSize);
        GWEN_EXPORT void SetWindowMinimized(void* pPtr, bool bMinimized);
    }

}

#endif // ifndef GWEN_PLATFORM_H
