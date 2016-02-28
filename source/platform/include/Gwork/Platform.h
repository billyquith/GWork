/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_PLATFORM_H
#define GWK_PLATFORM_H

#include <Gwork/Exports.h>
#include <string>

#if defined(GWK_NOT_NATIVE_PLATFORM)
#   define GWK_PLATFORM_EXTERNAL       // Platform implemented outside Gwork.
#elif defined(_WIN32)
#   define GWK_PLATFORM_WINDOWS        // Use Windows API.
#else
#   define GWK_PLATFORM_NULL           // Most features not implemented, but does link.
#endif


namespace Gwk
{
    struct Point;
    
    //
    // All strings are UTF-8 for Unicode. This is backwards compatible with ASCII.
    // See: http://www.utf8everywhere.org
    //
    typedef std::string     String;
    typedef char            UnicodeChar;

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
                                               const Gwk::String& strWindowTitle);
        GWK_EXPORT void DestroyPlatformWindow(void* pPtr);
        GWK_EXPORT void SetBoundsPlatformWindow(void* pPtr, int x, int y, int w, int h);
        // Returns true for redraw.
        GWK_EXPORT bool MessagePump(void* pWindow);
        GWK_EXPORT bool HasFocusPlatformWindow(void* pPtr);
        GWK_EXPORT void SetWindowMaximized(void* pPtr, bool bMaximized, Gwk::Point& pNewPos,
                                            Gwk::Point& pNewSize);
        GWK_EXPORT void SetWindowMinimized(void* pPtr, bool bMinimized);
    }

}

#endif // ifndef GWK_PLATFORM_H
