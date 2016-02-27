
#pragma once
#ifndef GWEN_CONTROLS_DIALOGS_FILEOPEN_H
#define GWEN_CONTROLS_DIALOGS_FILEOPEN_H

#include <Gwen/Gwen.h>

namespace Gwen
{
    namespace Dialogs
    {
        //
        // The REAL function.
        //
        void GWEN_EXPORT FileOpenEx(bool bUseSystem, const String& Name, const String& StartPath,
                                    const String& Extension, Gwen::Event::Handler* pHandler = NULL,
                                    Gwen::Event::Handler::FunctionWithInformation fnCallback =
                                        NULL);

        //! Create a file open dialog box.
        //! @param bUseSystem : If true dialog may use the system's modal dialog - which
        //!                     will steal focus and pause the rest of GWEN until it's continued.
        //!
        //! Usage:
        //! ~~~~
        //!     Gwen::Dialogs::FileOpen( true, "Open Map", "C:/my/folder/", "My Map Format|*.bmf",
        //!                              this, &MyClass::OpenFilename );
        //! ~~~~
        //!
        //! @note Templated function simply to avoid having to manually cast the
        //!       callback function.
        //!
        template <typename A>
        void FileOpen(bool bUseSystem, const String& Name, const String& StartPath,
                      const String& Extension,
                      Gwen::Event::Handler* pHandler = NULL, A fnCallback = NULL)
        {
            FileOpenEx(bUseSystem, Name, StartPath, Extension, pHandler,
                       static_cast<Gwen::Event::Handler::FunctionWithInformation>(fnCallback));
        }

    }
}

#endif // ifndef GWEN_CONTROLS_DIALOGS_FILEOPEN_H
