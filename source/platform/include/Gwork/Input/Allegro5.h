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
            :   m_EventListener(NULL)
            ,   m_MouseX(0)
            ,   m_MouseY(0)
            {
            }

            void Initialize(IInputEventListener* el)
            {
                m_EventListener = el;
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

                case ALLEGRO_KEY_LCTRL:
                    return Gwk::Key::Control;

                case ALLEGRO_KEY_ALT:
                    return Gwk::Key::Alt;

                case ALLEGRO_KEY_LSHIFT:
                    return Gwk::Key::Shift;

                case ALLEGRO_KEY_RCTRL:
                    return Gwk::Key::Control;

                case ALLEGRO_KEY_ALTGR:
                    return Gwk::Key::Alt;

                case ALLEGRO_KEY_RSHIFT:
                    return Gwk::Key::Shift;
                }

                return Gwk::Key::Invalid;
            }

            bool ProcessMessage(const ALLEGRO_EVENT& event)
            {
                if (!m_EventListener)
                    return false;

                switch (event.type)
                {
                case ALLEGRO_EVENT_MOUSE_AXES:
                    {
                        const int dx = event.mouse.dx;
                        const int dy = event.mouse.dy;

                        if (event.mouse.dz != 0)
                            return m_EventListener->InputMouseWheel(event.mouse.dz*60);

                        m_MouseX = event.mouse.x;
                        m_MouseY = event.mouse.y;
                        return m_EventListener->InputMouseMoved(m_MouseX, m_MouseY, dx, dy);
                    }

                case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                    return m_EventListener->InputMouseButton(event.mouse.button-1,
                                                      event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);

                case ALLEGRO_EVENT_KEY_CHAR:
                    return m_EventListener->InputCharacter(event.keyboard.unichar);

                case ALLEGRO_EVENT_KEY_DOWN:
                case ALLEGRO_EVENT_KEY_UP:
                    {
                        bool bPressed = (event.type == ALLEGRO_EVENT_KEY_DOWN);

                        if (event.keyboard.keycode
                            && bPressed
                            && event.keyboard.keycode >= 'a'
                            && event.keyboard.keycode <= 'z')
                        {
                            return m_EventListener->InputCharacter(event.keyboard.keycode);
                        }

                        const unsigned char iKey = TranslateKeyCode(event.keyboard.keycode);
                        return m_EventListener->InputKey(iKey, bPressed);
                    }
                }

                return false;
            }

        protected:

            IInputEventListener *m_EventListener;
            int m_MouseX;
            int m_MouseY;

        };


    }
}
#endif // ifndef GWK_INPUT_ALLEGRO_H
