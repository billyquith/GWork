/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
        //
        //! Callback function, for successful file save.
        //
        typedef void (Event::Handler::*FileSaveSuccessCallback)(const String& filename);

        //
        // The REAL function.
        //
        void GWK_EXPORT FileSaveEx(bool bUseSystem, const String& Name, const String& StartPath,
                                    const String& Extension, Gwk::Event::Handler* pHandler = NULL,
                                    Gwk::Event::Handler::FunctionWithInformation fnCallback =
                                        NULL);

        //! Open a dialog to save a file.
        //!
        //! @param bUseSystem : If true dialog may use the system's modal dialog - which
        //!                     will steal focus and pause the rest of Gwork until it's continued.
        //!
        //! Usage:
        //! ~~~~
        //!     Gwk::Dialogs::FileSave( true, "Save Map", "C:/my/folder/", "My Map Format|*.bmf",
        //!                              this, &MyClass::OpenFilename );
        //! ~~~~
        //!
        //! @note Templated function simply to avoid having to manually cast the
        //!       callback function.
        //
        template <typename A>
        void FileSave(bool bUseSystem, const String& Name, const String& StartPath,
                      const String& Extension, Gwk::Event::Handler* pHandler = NULL,
                      A fnCallback = NULL)
        {
            FileSaveEx(bUseSystem, Name, StartPath, Extension, pHandler,
                       static_cast<Gwk::Event::Handler::FunctionWithInformation>(fnCallback));
        }

    }
}

#endif // ifndef GWK_CONTROLS_DIALOGS_FILESAVE_H
