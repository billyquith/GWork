/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include "Gwork/Gwork.h"
#include "Gwork/Platform.h"
#include "Gwork/Controls/Dialogs/FolderOpen.h"

using namespace Gwk;
using namespace Gwk::Dialogs;

void Gwk::Dialogs::FolderOpenEx(bool bUseSystem, const String& Name, const String& StartPath,
                                 Gwk::Event::Handler* pHandler,
                                 Gwk::Event::Handler::FunctionWithInformation fnCallback)
{
    if (bUseSystem && Gwk::Platform::FolderOpen(Name, StartPath, pHandler, fnCallback))
        return;

    //
    // TODO: SHOW Gwork FOLDER SELECTION DIALOG
    //
}
