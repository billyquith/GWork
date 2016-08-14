/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUT_ALLEGRO_H
#define GWK_INPUT_ALLEGRO_H

#include <Gwork/InputEventListener.h>
#include <allegro5/allegro.h>

namespace Gwk
{
    namespace Input
    {
        class Allegro
        {
        public:

            Allegro()
            :   m_eventListener(nullptr)
            ,   m_mouseX(0)
            ,   m_mouseY(0)
            {
            }

            void Initialize(IInputEventListener* el)
            {
                m_eventListener = el;
            }

            unsigned char TranslateKeyCode(int iKeyCode)
            {
                switch (iKeyCode)
                {
                case ALLEGRO_KEY_BACKSPACE:
                    return Gwk::Key::Backspace;

                case ALLEGRO_KEY_ENTER:
                    return Gwk::Key::Return;

                case ALLEGRO_KEY_ESCAPE:
                    return Gwk::Key::Escape;

                case ALLEGRO_KEY_TAB:
                    return Gwk::Key::Tab;

                case ALLEGRO_KEY_SPACE:
                    return Gwk::Key::Space;

                case ALLEGRO_KEY_UP:
                    return Gwk::Key::Up;

                case ALLEGRO_KEY_DOWN:
                    return Gwk::Key::Down;

                case ALLEGRO_KEY_LEFT:
                    return Gwk::Key::Left;

                case ALLEGRO_KEY_RIGHT:
                    return Gwk::Key::Right;

                case ALLEGRO_KEY_HOME:
                    return Gwk::Key::Home;

                case ALLEGRO_KEY_END:
                    return Gwk::Key::End;

                case ALLEGRO_KEY_DELETE:
                    return Gwk::Key::Delete;

                case ALLEGRO_KEY_LSHIFT:
                    return Gwk::Key::Shift;

                case ALLEGRO_KEY_LCTRL:
                    return Gwk::Key::Control;

                case ALLEGRO_KEY_ALT:
                    return Gwk::Key::Alt;

                case ALLEGRO_KEY_COMMAND:
                    return Gwk::Key::Command;

                case ALLEGRO_KEY_RSHIFT:
                    return Gwk::Key::Shift;
                        
                case ALLEGRO_KEY_RCTRL:
                    return Gwk::Key::Control;

                case ALLEGRO_KEY_ALTGR:
                    return Gwk::Key::Alt;
                        
                default: ;
                }

                return Gwk::Key::Invalid;
            }

            bool ProcessMessage(const ALLEGRO_EVENT& event)
            {
                if (!m_eventListener)
                    return false;

                switch (event.type)
                {
                case ALLEGRO_EVENT_MOUSE_AXES:
                    {
                        const int dx = event.mouse.dx;
                        const int dy = event.mouse.dy;

                        if (event.mouse.dz != 0)
                            return m_eventListener->InputMouseWheel(event.mouse.dz*60);

                        m_mouseX = event.mouse.x;
                        m_mouseY = event.mouse.y;
                        return m_eventListener->InputMouseMoved(m_mouseX, m_mouseY, dx, dy);
                    }

                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    return m_eventListener->InputMouseButton(
                                                    event.mouse.button-1,
                                                    event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);

                case ALLEGRO_EVENT_KEY_CHAR:
                    return m_eventListener->InputCharacter(event.keyboard.unichar);

                case ALLEGRO_EVENT_KEY_DOWN:
                case ALLEGRO_EVENT_KEY_UP:
                    {
                        const bool bPressed = (event.type == ALLEGRO_EVENT_KEY_DOWN);

                        if (event.keyboard.keycode != 0
                            && bPressed
                            && event.keyboard.keycode >= ALLEGRO_KEY_A
                            && event.keyboard.keycode <= ALLEGRO_KEY_Z)
                        {
                            return m_eventListener->InputCharacter(event.keyboard.keycode);
                        }

                        const unsigned char key = TranslateKeyCode(event.keyboard.keycode);
                        return key == Gwk::Key::Invalid
                               ? false
                               : m_eventListener->InputModifierKey(key, bPressed);
                    }
                }

                return false;
            }

        protected:

            IInputEventListener *m_eventListener;
            int m_mouseX;
            int m_mouseY;

        };
    }
}

#endif // ifndef GWK_INPUT_ALLEGRO_H
