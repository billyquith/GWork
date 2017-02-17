/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_DIALOGS_FILEOPEN_H
#define GWK_CONTROLS_DIALOGS_FILEOPEN_H

#include <Gwork/Gwork.h>

namespace Gwk
{
    namespace Dialogs
    {
        //! Create a file open dialog box.
        //! @param bUseSystem : If true dialog may use the system's modal dialog - which
        //!                     will steal focus and pause the rest of Gwork until it's continued.
        //!
        //! Usage:
        //! ~~~~
        //!     String fileOpenOut;
        //!     if (Gwk::Dialogs::FileOpen( true, "Open Map", "C:/my/folder/", "My Map Format|*.bmf",
        //!                                 fileOpenOut ))
        //!     // .. do something with fileOpenOut ..
        //! ~~~~
        //!
        //! @note Templated function simply to avoid having to manually cast the
        //!       callback function.
        //!
        bool FileOpen(bool bUseSystem, const String& Name, const String& StartPath,
                      const String& Extension, String& fileOpenOut);
    }
}

#endif // ifndef GWK_CONTROLS_DIALOGS_FILEOPEN_H
