/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_DIALOGS_FILESAVE_H
#define GWK_CONTROLS_DIALOGS_FILESAVE_H

#include <Gwork/Gwork.h>

namespace Gwk
{
    namespace Dialogs
    {
        //! Open a dialog to save a file.
        //!
        //! @param bUseSystem : If true dialog may use the system's modal dialog - which
        //!                     will steal focus and pause the rest of Gwork until it's continued.
        //!
        //! Usage:
        //! ~~~~
        //!     String fileSaveOut;
        //!     Gwk::Dialogs::FileSave( true, "Save Map", "C:/my/folder/", "My Map Format|*.bmf",
        //!                             fileSaveOut );
        //! ~~~~
        //!
        //! @note Templated function simply to avoid having to manually cast the
        //!       callback function.
        //
        bool FileSave(bool bUseSystem, const String& Name, const String& StartPath,
                      const String& Extension, String& fileSaveOut);
    }
}

#endif // ifndef GWK_CONTROLS_DIALOGS_FILESAVE_H
