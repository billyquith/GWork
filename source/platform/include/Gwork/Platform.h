/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_PLATFORM_H
#define GWK_PLATFORM_H

#include <Gwork/PlatformTypes.h>
#include <string>

namespace Gwk
{
    namespace Platform
    {
        //! Go to sleep for a time. Stops CPU hogging.
        //! @param ms : Number of milliseconds to sleep for.
        GWK_EXPORT void Sleep(unsigned int ms);

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

        //
        // System Dialogs (Can return false if unhandled)
        //
        GWK_EXPORT bool FileOpen(const String& Name, const String& StartPath,
                                 const String& Extension, String& filePathOut);
        GWK_EXPORT bool FileSave(const String& Name, const String& StartPath,
                                 const String& Extension, String& filePathOut);
        GWK_EXPORT bool FolderOpen(const String& Name, const String& StartPath,
                                   String& filePathOut);

        //
        // Window Creation
        //
        GWK_EXPORT void* CreatePlatformWindow(int x, int y, int w, int h,
                                              const Gwk::String& windowTitle);
        GWK_EXPORT void DestroyPlatformWindow(void* pPtr);
        GWK_EXPORT void SetBoundsPlatformWindow(void* pPtr, int x, int y, int w, int h);
        // Returns true for redraw.
        GWK_EXPORT bool MessagePump(void* pWindow);
        GWK_EXPORT bool HasFocusPlatformWindow(void* pPtr);
        GWK_EXPORT void SetWindowMaximized(void* pPtr, bool bMaximized, Gwk::Point& pNewPos,
                                           Gwk::Point& pNewSize);
        GWK_EXPORT void SetWindowMinimized(void* pPtr, bool bMinimized);
        
        //! If the user created the window, notify the plaform so it can do its housekeeping.
        GWK_EXPORT void SetPlatformWindow(void* handle);
    }

}

#endif // ifndef GWK_PLATFORM_H
