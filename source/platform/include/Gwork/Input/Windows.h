/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */
#ifndef GWK_INPUT_WINDOWS_H
#define GWK_INPUT_WINDOWS_H

#include <Gwork/InputHandler.h>
#include <Gwork/Gwork.h>
#include <Gwork/Controls/Canvas.h>

#include <windows.h>

namespace Gwk
{
    namespace Input
    {
        class Windows
        {
        public:

            Windows()
            {
                m_canvas = nullptr;
                m_mouseX = 0;
                m_mouseY = 0;
            }

            void Initialize(Gwk::Controls::Canvas* c)
            {
                m_canvas = c;
            }

            bool ProcessMessage(MSG msg)
            {
                if (!m_canvas)
                    return false;

                switch (msg.message)
                {
                // case WM_NCLBUTTONDOWN:
                case WM_SYSCOMMAND:
                    if (msg.message == WM_SYSCOMMAND && msg.wParam != SC_CLOSE)
                        return false;

                    return m_canvas->InputQuit();

                case WM_MOUSEMOVE:
                    {
                        int x = (signed short)LOWORD(msg.lParam);
                        int y = (signed short)HIWORD(msg.lParam);
                        int dx = x-m_mouseX;
                        int dy = y-m_mouseY;
                        m_mouseX = x;
                        m_mouseY = y;
                        return m_canvas->InputMouseMoved(x, y, dx, dy);
                    }

                case WM_CHAR:
                    {
                        Gwk::UnicodeChar chr = (Gwk::UnicodeChar)msg.wParam;
                        return m_canvas->InputCharacter(chr);
                    }

#ifdef WM_MOUSEWHEEL

                case WM_MOUSEWHEEL:
                    return m_canvas->InputMouseWheel((short)HIWORD(msg.wParam));

#endif

                case WM_LBUTTONDOWN:
                    SetCapture(msg.hwnd);
                    return m_canvas->InputMouseButton(0, true);

                case WM_LBUTTONUP:
                    ReleaseCapture();
                    return m_canvas->InputMouseButton(0, false);

                case WM_RBUTTONDOWN:
                    SetCapture(msg.hwnd);
                    return m_canvas->InputMouseButton(1, true);

                case WM_RBUTTONUP:
                    ReleaseCapture();
                    return m_canvas->InputMouseButton(1, false);

                case WM_MBUTTONDOWN:
                    SetCapture(msg.hwnd);
                    return m_canvas->InputMouseButton(2, true);

                case WM_MBUTTONUP:
                    ReleaseCapture();
                    return m_canvas->InputMouseButton(2, false);

                case WM_LBUTTONDBLCLK:
                case WM_RBUTTONDBLCLK:
                case WM_MBUTTONDBLCLK:
                    // Filter out those events from the application
                    return true;

                case WM_KEYDOWN:
                case WM_KEYUP:
                    {
                        bool bDown = msg.message == WM_KEYDOWN;
                        int iKey = -1;

                        // These aren't sent by WM_CHAR when CTRL is down - but
                        // we need
                        // them internally for copy and paste etc..
                        if (bDown && GetKeyState(VK_CONTROL)&0x80 && msg.wParam >= 'A' &&
                            msg.wParam <=
                            'Z')
                        {
                            Gwk::UnicodeChar chr = (Gwk::UnicodeChar)msg.wParam;
                            return m_canvas->InputCharacter(chr);
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
                            return m_canvas->InputModifierKey(iKey, bDown);

                        break;
                    }

                default:
                    break;
                }

                return false;
            }

        protected:

            Gwk::Controls::Canvas* m_canvas;
            int m_mouseX;
            int m_mouseY;

        };


    }
}
#endif // ifndef GWK_INPUT_WINDOWS_H
