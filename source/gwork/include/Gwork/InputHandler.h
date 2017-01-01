/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUTHANDLER_H
#define GWK_INPUTHANDLER_H

#include <Gwork/InputEventListener.h>
#include <queue>

namespace Gwk
{
    namespace Controls
    {
        class Base;
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
        bool GWK_EXPORT         IsKeyDown(int key);
        bool GWK_EXPORT         IsLeftMouseDown();
        bool GWK_EXPORT         IsRightMouseDown();
        Gwk::Point GWK_EXPORT   GetMousePosition();

        inline bool IsShiftDown()
        {
            return IsKeyDown(Gwk::Key::Shift);
        }

        inline bool IsControlDown()
        {
            return IsKeyDown(Gwk::Key::Control);
        }

        // Does copy, paste etc
        bool GWK_EXPORT DoSpecialKeys(Controls::Base* canvas, Gwk::UnicodeChar chr);
        bool GWK_EXPORT HandleAccelerator(Controls::Base* canvas, Gwk::UnicodeChar chr);

        // Send input to canvas for study
        void GWK_EXPORT OnMouseMoved(Controls::Base* canvas, int x, int y,
                                     int deltaX, int deltaY);
        bool GWK_EXPORT OnMouseClicked(Controls::Base* canvas, int iButton, bool bDown);
        bool GWK_EXPORT OnKeyEvent(Controls::Base* canvas, int iKey, bool bDown);
        void GWK_EXPORT OnCanvasThink(Controls::Base* control);
    }
}

#endif // ifndef GWK_INPUTHANDLER_H
