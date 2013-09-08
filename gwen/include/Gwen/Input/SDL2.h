/*
 *  GWEN
 *  Copyright (c) 2011 Facepunch Studios
 *  See license in Gwen.h
 *  SDL2 Added by BQ.
 */
#ifndef GWEN_INPUT_SDL2_H
#define GWEN_INPUT_SDL2_H

#include "Gwen/InputHandler.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls/Canvas.h"

#ifdef _WIN32
#   include <SDL.h>
#else
#   include <SDL2/SDL.h>
#endif


namespace Gwen
{
    namespace Input
    {
        class SDL2
        {
        public:

            SDL2()
            {
                m_Canvas = NULL;
            }

            void Initialize(Gwen::Controls::Canvas* c)
            {
                m_Canvas = c;
            }

            bool ProcessEvent(SDL_Event* event)
            {
                if (!m_Canvas)
                    return false;

                switch (event->type)
                {
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                    {
                        SDL_KeyboardEvent *ke = &event->key;
                        int iKey = -1;
                        SDL_Scancode scancode = ke->keysym.scancode;

                        switch (scancode)
                        {
                        case SDL_SCANCODE_RETURN:       iKey = Gwen::Key::Return; break;
                        case SDL_SCANCODE_BACKSPACE:    iKey = Gwen::Key::Backspace; break;
                        case SDL_SCANCODE_DELETE:       iKey = Gwen::Key::Delete; break;
                        case SDL_SCANCODE_LEFT:         iKey = Gwen::Key::Left; break;
                        case SDL_SCANCODE_RIGHT:        iKey = Gwen::Key::Right; break;
                        case SDL_SCANCODE_LSHIFT:       iKey = Gwen::Key::Shift; break;
                        case SDL_SCANCODE_RSHIFT:       iKey = Gwen::Key::Shift; break;
                        case SDL_SCANCODE_TAB:          iKey = Gwen::Key::Tab; break;
                        case SDL_SCANCODE_SPACE:        iKey = Gwen::Key::Space; break;
                        case SDL_SCANCODE_HOME:         iKey = Gwen::Key::Home; break;
                        case SDL_SCANCODE_END:          iKey = Gwen::Key::End; break;
                        case SDL_SCANCODE_LCTRL:        iKey = Gwen::Key::Control; break;
                        case SDL_SCANCODE_RCTRL:        iKey = Gwen::Key::Control; break;
                        case SDL_SCANCODE_UP:           iKey = Gwen::Key::Up; break;
                        case SDL_SCANCODE_DOWN:         iKey = Gwen::Key::Down; break;
                        case SDL_SCANCODE_ESCAPE:       iKey = Gwen::Key::Escape; break;
                        case SDL_SCANCODE_LALT:         iKey = Gwen::Key::Alt; break;
                        case SDL_SCANCODE_RALT:         iKey = Gwen::Key::Alt; break;
                        default:                        return false;
                        }

                        return m_Canvas->InputKey(iKey, ke->state != 0);
                    }

                case SDL_TEXTINPUT:
                    {
                        // TODO: This will probably need fixing for UTF-8.
                        return m_Canvas->InputCharacter(event->text.text[0]);
                    }

                case SDL_MOUSEMOTION:
                    {
                        SDL_MouseMotionEvent* E = &event->motion;
                        return m_Canvas->InputMouseMoved(E->x, E->y, E->xrel, E->yrel);
                    }

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    {
                        SDL_MouseButtonEvent* E = &event->button;
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

                        return m_Canvas->InputMouseButton(Button, E->state != 0);
                    }

                case SDL_MOUSEWHEEL:
                    {
                        SDL_MouseWheelEvent* E = &event->wheel;
                        return m_Canvas->InputMouseWheel(E->y);
                    }

                default:
                    return false;
                }
            }

        protected:

            Gwen::Controls::Canvas* m_Canvas;

        };


    }
}
#endif // GWEN_INPUT_SDL2_H
