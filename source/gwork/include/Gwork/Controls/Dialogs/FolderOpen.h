/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
        //
        // The REAL function.
        //
        void GWK_EXPORT FolderOpenEx(bool bUseSystem, const String& Name, const String& StartPath,
                                      Gwk::Event::Handler* pHandler = NULL,
                                      Gwk::Event::Handler::FunctionWithInformation fnCallback =
                                          NULL);

        //! Open a dialog to choose a folder.
        //!
        //! @param bUseSystem : If true dialog may use the system's modal dialog - which
        //!                     will steal focus and pause the rest of Gwork until it's continued.
        //! Usage:
        //! ~~~~
        //!  Gwk::Dialogs::FolderOpen( true, "Open Map folder", "C:/my/folder/",
        //!                             "My Map Format|*.bmf", this, &MyClass::OpenFilename );
        //! ~~~~
        //! @note Templated function simply to avoid having to manually cast the
        //!       callback function.
        //
        template <typename A>
        void FolderOpen(bool bUseSystem, const String& Name, const String& StartPath,
                        Gwk::Event::Handler* pHandler = NULL, A fnCallback = NULL)
        {
            FolderOpenEx(bUseSystem, Name, StartPath, pHandler,
                         static_cast<Gwk::Event::Handler::FunctionWithInformation>(fnCallback));
        }

    }
}

#endif // ifndef GWK_CONTROLS_DIALOGS_FOLDEROPEN_H
