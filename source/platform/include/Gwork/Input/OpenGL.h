/*
 *  Gwork
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUT_OPENGL_GLFW_H
#define GWK_INPUT_OPENGL_GLFW_H

#include <Gwork/InputEventListener.h>
#include <GLFW/glfw3.h>

namespace Gwk
{
    namespace Input
    {
        class GLFW
        {
        public:
            
            struct KeyEvent {
                int key, scancode, action, mods;
            };

            struct MouseEvent {
                double x, y;
            };

            GLFW()
            :   m_eventListener(nullptr)
            ,   m_mouse(-1, -1)
            {}

            void Initialize(IInputEventListener* c)
            {
                m_eventListener = c;
            }

            unsigned char TranslateKeyCode(int iKeyCode)
            {
                switch (iKeyCode)
                {
                case GLFW_KEY_BACKSPACE:
                    return Gwk::Key::Backspace;

                case GLFW_KEY_ENTER:
                    return Gwk::Key::Return;

                case GLFW_KEY_ESCAPE:
                    return Gwk::Key::Escape;

                case GLFW_KEY_TAB:
                    return Gwk::Key::Tab;

                case GLFW_KEY_SPACE:
                    return Gwk::Key::Space;

                case GLFW_KEY_UP:
                    return Gwk::Key::Up;

                case GLFW_KEY_DOWN:
                    return Gwk::Key::Down;

                case GLFW_KEY_LEFT:
                    return Gwk::Key::Left;

                case GLFW_KEY_RIGHT:
                    return Gwk::Key::Right;

                case GLFW_KEY_HOME:
                    return Gwk::Key::Home;

                case GLFW_KEY_END:
                    return Gwk::Key::End;

                case GLFW_KEY_DELETE:
                    return Gwk::Key::Delete;

                case GLFW_KEY_LEFT_CONTROL:
                    return Gwk::Key::Control;

                case GLFW_KEY_LEFT_ALT:
                    return Gwk::Key::Alt;

                case GLFW_KEY_LEFT_SHIFT:
                    return Gwk::Key::Shift;

                case GLFW_KEY_RIGHT_CONTROL:
                    return Gwk::Key::Control;

                case GLFW_KEY_RIGHT_ALT:
                    return Gwk::Key::Alt;

                case GLFW_KEY_RIGHT_SHIFT:
                    return Gwk::Key::Shift;
                        
                default: ;
                }

                return Gwk::Key::Invalid;
            }
            
            bool ProcessKeyEvent(const KeyEvent& event)
            {
                if (!m_eventListener)
                    return false;
                
                const bool bPressed = (event.action == GLFW_PRESS);
                const char keyCode = event.key;
                const bool control = (event.mods & GLFW_MOD_CONTROL) != 0;
                
                if (control && bPressed && keyCode >= 'a' && keyCode <= 'z')
                    return m_eventListener->InputCharacter(keyCode);
                
                const unsigned char iKey = TranslateKeyCode(keyCode);
                return m_eventListener->InputModifierKey(iKey, bPressed);
            }

            bool ProcessMouseMove(int x, int y)
            {
                if (m_mouse.x < 0)
                    m_lastMouse = m_mouse;
                
                m_mouse.set(x, y);
                const Point delta(m_lastMouse - m_mouse);
                m_lastMouse = m_mouse;
                
                return m_eventListener->InputMouseMoved(m_mouse.x, m_mouse.y,
                                                        delta.x, delta.y);
            }
            
            bool ProcessMouseButtons(int button, int action, int mods)
            {
                return m_eventListener->InputMouseButton(button, action == GLFW_PRESS);
            }

            bool ProcessScroll(float sx, float sy)
            {
                return m_eventListener->InputMouseWheel(sy);
            }

        protected:

            IInputEventListener *m_eventListener;
            Point m_mouse, m_lastMouse;

        };

    } // Input
} // Gwk

#endif // ifndef GWK_INPUT_OPENGL_GLFW_H
