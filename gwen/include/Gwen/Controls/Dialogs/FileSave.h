#pragma once
#ifndef GWEN_CONTROLS_DIALOGS_FILESAVE_H
#define GWEN_CONTROLS_DIALOGS_FILESAVE_H

#include <Gwen/Gwen.h>

namespace Gwen
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
        void GWEN_EXPORT FileSaveEx(bool bUseSystem, const String& Name, const String& StartPath,
                                    const String& Extension, Gwen::Event::Handler* pHandler = NULL,
                                    Gwen::Event::Handler::FunctionWithInformation fnCallback =
                                        NULL);

        //! Open a dialog to save a file.
        //!
        //! @param bUseSystem : If true dialog may use the system's modal dialog - which
        //!                     will steal focus and pause the rest of GWEN until it's continued.
        //!
        //! Usage:
        //! ~~~~
        //!     Gwen::Dialogs::FileSave( true, "Save Map", "C:/my/folder/", "My Map Format|*.bmf",
        //!                              this, &MyClass::OpenFilename );
        //! ~~~~
        //!
        //! @note Templated function simply to avoid having to manually cast the
        //!       callback function.
        //
        template <typename A>
        void FileSave(bool bUseSystem, const String& Name, const String& StartPath,
                      const String& Extension, Gwen::Event::Handler* pHandler = NULL,
                      A fnCallback = NULL)
        {
            FileSaveEx(bUseSystem, Name, StartPath, Extension, pHandler,
                       static_cast<Gwen::Event::Handler::FunctionWithInformation>(fnCallback));
        }

    }
}

#endif // ifndef GWEN_CONTROLS_DIALOGS_FILESAVE_H
