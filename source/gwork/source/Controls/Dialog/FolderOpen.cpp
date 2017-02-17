/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Platform.h>
#include <Gwork/Controls/Dialogs/FolderOpen.h>


bool Gwk::Dialogs::FolderOpen(bool bUseSystem, const String& Name, const String& StartPath,
                              String& folderChosenOut)
{
    if (bUseSystem)
    {
        return Gwk::Platform::FolderOpen(Name, StartPath, folderChosenOut);
    }

    // TODO: SHOW Gwork FOLDER SELECTION DIALOG
    
    return false;
}
