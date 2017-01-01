/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUTEVENTLISTENER_H
#define GWK_INPUTEVENTLISTENER_H

#include <Gwork/PlatformTypes.h>

namespace Gwk
{
    namespace Key
    {
        enum : char
        {
            Invalid = 0,
            Return = 1,
            Backspace = 2,
            Delete = 3,
            Left = 4,
            Right = 5,
            Shift = 6,
            Tab = 7,
            Space = 8,
            Home = 9,
            End = 10,
            Control = 11,
            Up = 12,
            Down = 13,
            Escape = 14,
            Alt = 15,
            Command = 16,   // Mac command/clover
            KeysCount //!< number of keys
        };
    }
    
    class IInputEventListener
    {
    public:
        virtual ~IInputEventListener() {}
        
        virtual bool InputMouseMoved(int x, int y, int deltaX, int deltaY) = 0;
        virtual bool InputMouseButton(int iButton, bool bDown) = 0;
        virtual bool InputModifierKey(int iKey, bool bDown) = 0;
        virtual bool InputCharacter(Gwk::UnicodeChar chr) = 0;
        virtual bool InputMouseWheel(int val) = 0;
        virtual bool InputQuit() = 0;
    };
}

#endif // ifndef GWK_INPUTEVENTLISTENER_H
