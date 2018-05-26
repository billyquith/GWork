/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 *  SDL2 Added by BQ.
 */
#ifndef GWK_INPUT_SDL2_H
#define GWK_INPUT_SDL2_H

#include <Gwork/InputHandler.h>
#include <Gwork/Gwork.h>
#include <Gwork/InputEventListener.h>
#include <SDL.h>


namespace Gwk
{
    namespace Input
    {
        class SDL2
        {
        public:

            SDL2()
            {
                m_listener = nullptr;
            }

            void Initialize(Gwk::IInputEventListener* c)
            {
                m_listener = c;
            }

            bool ProcessEvent(const SDL_Event* event)
            {
                if (!m_listener)
                    return false;

                switch (event->type)
                {
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                    {
                        const SDL_KeyboardEvent *ke = &event->key;
                        int iKey = -1;
                        SDL_Scancode scancode = ke->keysym.scancode;

                        switch (scancode)
                        {
                        case SDL_SCANCODE_RETURN:       iKey = Gwk::Key::Return; break;
                        case SDL_SCANCODE_BACKSPACE:    iKey = Gwk::Key::Backspace; break;
                        case SDL_SCANCODE_DELETE:       iKey = Gwk::Key::Delete; break;
                        case SDL_SCANCODE_LEFT:         iKey = Gwk::Key::Left; break;
                        case SDL_SCANCODE_RIGHT:        iKey = Gwk::Key::Right; break;
                        case SDL_SCANCODE_LSHIFT:       iKey = Gwk::Key::Shift; break;
                        case SDL_SCANCODE_RSHIFT:       iKey = Gwk::Key::Shift; break;
                        case SDL_SCANCODE_TAB:          iKey = Gwk::Key::Tab; break;
                        case SDL_SCANCODE_SPACE:        iKey = Gwk::Key::Space; break;
                        case SDL_SCANCODE_HOME:         iKey = Gwk::Key::Home; break;
                        case SDL_SCANCODE_END:          iKey = Gwk::Key::End; break;
                        case SDL_SCANCODE_LCTRL:        iKey = Gwk::Key::Control; break;
                        case SDL_SCANCODE_RCTRL:        iKey = Gwk::Key::Control; break;
                        case SDL_SCANCODE_UP:           iKey = Gwk::Key::Up; break;
                        case SDL_SCANCODE_DOWN:         iKey = Gwk::Key::Down; break;
                        case SDL_SCANCODE_ESCAPE:       iKey = Gwk::Key::Escape; break;
                        case SDL_SCANCODE_LALT:         iKey = Gwk::Key::Alt; break;
                        case SDL_SCANCODE_RALT:         iKey = Gwk::Key::Alt; break;
                        default:                        return false;
                        }

                        return m_listener->InputModifierKey(iKey, ke->state != 0);
                    }

                case SDL_TEXTINPUT:
                    {
                        // TODO: This will probably need fixing for UTF-8.
                        return m_listener->InputCharacter(event->text.text[0]);
                    }

                case SDL_MOUSEMOTION:
                    {
                        const SDL_MouseMotionEvent* E = &event->motion;
                        return m_listener->InputMouseMoved(E->x, E->y, E->xrel, E->yrel);
                    }

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    {
                        const SDL_MouseButtonEvent* E = &event->button;
                        int Button = -1;

                        switch (E->button)
                        {
                        case SDL_BUTTON_LEFT:
                            Button = 0;
                            break;

                        case SDL_BUTTON_MIDDLE:
                            Button = 2;
                            break;

                        case SDL_BUTTON_RIGHT:
                            Button = 1;
                            break;

                        default:
                            return false;
                        }

                        return m_listener->InputMouseButton(Button, E->state != 0);
                    }

                case SDL_MOUSEWHEEL:
                    {
                        const SDL_MouseWheelEvent* E = &event->wheel;
                        return m_listener->InputMouseWheel(E->y);
                    }

                default:
                    return false;
                }
            }

        protected:

            Gwk::IInputEventListener* m_listener;

        };


    }
}
#endif // GWK_INPUT_SDL2_H
