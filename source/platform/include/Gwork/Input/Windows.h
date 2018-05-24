/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUT_WINDOWS_H
#define GWK_INPUT_WINDOWS_H

#include <Gwork/InputEventListener.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace Gwk
{
    namespace Input
    {
        //! Translate Windows system messages into Gwork events.
        class Windows
        {
        public:

            Windows(IInputEventListener* listener)
                :   m_eventListener(listener)
                ,   m_mouseX(0)
                ,   m_mouseY(0)
            {
            }

            bool ProcessMessage(MSG msg)
            {
                if (!m_eventListener)
                    return false;

                switch (msg.message)
                {
                // case WM_NCLBUTTONDOWN:
                case WM_SYSCOMMAND:
                    if (msg.message == WM_SYSCOMMAND && msg.wParam != SC_CLOSE)
                        return false;

                    return m_eventListener->InputQuit();

                case WM_MOUSEMOVE:
                    {
                        const int x = (signed short)LOWORD(msg.lParam);
                        const int y = (signed short)HIWORD(msg.lParam);
                        const int dx = x - m_mouseX;
                        const int dy = y - m_mouseY;
                        m_mouseX = x;
                        m_mouseY = y;
                        return m_eventListener->InputMouseMoved(x, y, dx, dy);
                    }

                case WM_CHAR:
                    {
                        const Gwk::UnicodeChar chr = (Gwk::UnicodeChar)msg.wParam;
                        return m_eventListener->InputCharacter(chr);
                    }

#ifdef WM_MOUSEWHEEL

                case WM_MOUSEWHEEL:
                    return m_eventListener->InputMouseWheel((short)HIWORD(msg.wParam));

#endif

                case WM_LBUTTONDOWN:
                    SetCapture(msg.hwnd);
                    return m_eventListener->InputMouseButton(0, true);

                case WM_LBUTTONUP:
                    ReleaseCapture();
                    return m_eventListener->InputMouseButton(0, false);

                case WM_RBUTTONDOWN:
                    SetCapture(msg.hwnd);
                    return m_eventListener->InputMouseButton(1, true);

                case WM_RBUTTONUP:
                    ReleaseCapture();
                    return m_eventListener->InputMouseButton(1, false);

                case WM_MBUTTONDOWN:
                    SetCapture(msg.hwnd);
                    return m_eventListener->InputMouseButton(2, true);

                case WM_MBUTTONUP:
                    ReleaseCapture();
                    return m_eventListener->InputMouseButton(2, false);

                case WM_LBUTTONDBLCLK:
                case WM_RBUTTONDBLCLK:
                case WM_MBUTTONDBLCLK:
                    // Filter out those events from the application
                    return true;

                case WM_KEYDOWN:
                case WM_KEYUP:
                    {
                        const bool bDown = msg.message == WM_KEYDOWN;
                        int iKey = -1;

                        // These aren't sent by WM_CHAR when CTRL is down - but
                        // we need them internally for copy and paste etc..
                        if (bDown
                            && (GetKeyState(VK_CONTROL) & 0x80) != 0
                            && msg.wParam >= 'A'
                            && msg.wParam <= 'Z')
                        {
                            const Gwk::UnicodeChar chr = (Gwk::UnicodeChar)msg.wParam;
                            return m_eventListener->InputCharacter(chr);
                        }

                        if (msg.wParam == VK_SHIFT)
                            iKey = Gwk::Key::Shift;
                        else if (msg.wParam == VK_RETURN)
                            iKey = Gwk::Key::Return;
                        else if (msg.wParam == VK_BACK)
                            iKey = Gwk::Key::Backspace;
                        else if (msg.wParam == VK_DELETE)
                            iKey = Gwk::Key::Delete;
                        else if (msg.wParam == VK_LEFT)
                            iKey = Gwk::Key::Left;
                        else if (msg.wParam == VK_RIGHT)
                            iKey = Gwk::Key::Right;
                        else if (msg.wParam == VK_TAB)
                            iKey = Gwk::Key::Tab;
                        else if (msg.wParam == VK_SPACE)
                            iKey = Gwk::Key::Space;
                        else if (msg.wParam == VK_HOME)
                            iKey = Gwk::Key::Home;
                        else if (msg.wParam == VK_END)
                            iKey = Gwk::Key::End;
                        else if (msg.wParam == VK_CONTROL)
                            iKey = Gwk::Key::Control;
                        else if (msg.wParam == VK_SPACE)
                            iKey = Gwk::Key::Space;
                        else if (msg.wParam == VK_UP)
                            iKey = Gwk::Key::Up;
                        else if (msg.wParam == VK_DOWN)
                            iKey = Gwk::Key::Down;

                        if (iKey != -1)
                            return m_eventListener->InputModifierKey(iKey, bDown);

                        break;
                    }

                default:
                    break;
                }

                return false;
            }

        protected:

            IInputEventListener* m_eventListener;
            int m_mouseX;
            int m_mouseY;

        };


    }
}
#endif // ifndef GWK_INPUT_WINDOWS_H
