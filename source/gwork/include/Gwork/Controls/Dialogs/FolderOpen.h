/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_DIALOGS_FOLDEROPEN_H
#define GWK_CONTROLS_DIALOGS_FOLDEROPEN_H

#include <Gwork/Gwork.h>

namespace Gwk
{
    namespace Dialogs
    {
        //! Open a dialog to choose a folder.
        //!
        //! @param bUseSystem : If true dialog may use the system's modal dialog - which
        //!                     will steal focus and pause the rest of Gwork until it's continued.
        //! Usage:
        //! ~~~~
        //!  String folderChosenOut;
        //!  Gwk::Dialogs::FolderOpen( true, "Open Map folder", "C:/my/folder/",
        //!                             "My Map Format|*.bmf", folderChosenOut );
        //! ~~~~
        //! @note Templated function simply to avoid having to manually cast the
        //!       callback function.
        //
        bool FolderOpen(bool bUseSystem, const String& Name, const String& StartPath,
                        String& folderChosenOut);
    }
}

#endif // ifndef GWK_CONTROLS_DIALOGS_FOLDEROPEN_H
