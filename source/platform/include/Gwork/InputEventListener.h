/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUTEVENTLISTENER_H
#define GWK_INPUTEVENTLISTENER_H

#include <Gwork/PlatformTypes.h>

namespace Gwk
{
    class IInputEventListener
    {
    public:
        virtual ~IInputEventListener() {}
        
        virtual bool InputMouseMoved(int x, int y, int deltaX, int deltaY) = 0;
        virtual bool InputMouseButton(int iButton, bool bDown) = 0;
        virtual bool InputKey(int iKey, bool bDown) = 0;
        virtual bool InputCharacter(Gwk::UnicodeChar chr) = 0;
        virtual bool InputMouseWheel(int val) = 0;
        virtual bool InputQuit() = 0;
    };
}

#endif // ifndef GWK_INPUTEVENTLISTENER_H
