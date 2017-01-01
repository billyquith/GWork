/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Platform.h>
#include <Gwork/Controls/Dialogs/FileOpen.h>


bool Gwk::Dialogs::FileOpen(bool bUseSystem, const String& Name, const String& StartPath,
                            const String& Extension, String& fileOpenOut)
{
    String fileOpened;
    
    if (bUseSystem)
    {
        return Gwk::Platform::FileOpen(Name, StartPath, Extension, fileOpened);
    }

    // TODO: SHOW Gwork FILE SELECTION DIALOG
    
    return false;
}
