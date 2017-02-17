/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Platform.h>
#include <Gwork/Controls/Dialogs/FileSave.h>


bool Gwk::Dialogs::FileSave(bool bUseSystem, const String& Name, const String& StartPath,
                            const String& Extension, String& fileOpenOut)
{
    if (bUseSystem)
    {
        return Gwk::Platform::FileSave(Name, StartPath, Extension, fileOpenOut);
    }
    
    // TODO: SHOW Gwork FILE SELECTION DIALOG
    
    return false;
}
