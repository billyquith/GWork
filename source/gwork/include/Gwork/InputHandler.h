/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUTHANDLER_H
#define GWK_INPUTHANDLER_H

#include <queue>
#include <Gwork/Gwork.h>

namespace Gwk
{
    namespace Controls
    {
        class Base;
    }

    namespace Key
    {
        const unsigned char Invalid = 0;
        const unsigned char Return = 1;
        const unsigned char Backspace = 2;
        const unsigned char Delete = 3;
        const unsigned char Left = 4;
        const unsigned char Right = 5;
        const unsigned char Shift = 6;
        const unsigned char Tab = 7;
        const unsigned char Space = 8;
        const unsigned char Home = 9;
        const unsigned char End = 10;
        const unsigned char Control = 11;
        const unsigned char Up = 12;
        const unsigned char Down = 13;
        const unsigned char Escape = 14;
        const unsigned char Alt = 15;

        const unsigned char Count = 16;
    }

    namespace Input
    {
        namespace Message
        {
            enum
            {
                Copy,
                Paste,
                Cut,
                Undo,
                Redo,
                SelectAll
            };


        }


        // For use in panels
        bool GWK_EXPORT        IsKeyDown(int iKey);
        bool GWK_EXPORT        IsLeftMouseDown();
        bool GWK_EXPORT        IsRightMouseDown();
        Gwk::Point GWK_EXPORT GetMousePosition();

        inline bool IsShiftDown()
        {
            return IsKeyDown(Gwk::Key::Shift);
        }

        inline bool IsControlDown()
        {
            return IsKeyDown(Gwk::Key::Control);
        }

        // Does copy, paste etc
        bool GWK_EXPORT DoSpecialKeys(Controls::Base* pCanvas, Gwk::UnicodeChar chr);
        bool GWK_EXPORT HandleAccelerator(Controls::Base* pCanvas, Gwk::UnicodeChar chr);

        // Send input to canvas for study
        void GWK_EXPORT OnMouseMoved(Controls::Base* pCanvas, int x, int y, int deltaX,
                                      int deltaY);
        bool GWK_EXPORT OnMouseClicked(Controls::Base* pCanvas, int iButton, bool bDown);
        bool GWK_EXPORT OnKeyEvent(Controls::Base* pCanvas, int iKey, bool bDown);
        void GWK_EXPORT OnCanvasThink(Controls::Base* pControl);


    }
}
#endif // ifndef GWK_INPUTHANDLER_H
