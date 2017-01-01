/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */
#ifndef GWK_INPUT_SDL13_H
#define GWK_INPUT_SDL13_H

#include <Gwork/InputHandler.h>
#include <Gwork/Gwork.h>
#include <Gwork/Controls/Canvas.h>

#ifdef _WIN32
#define UCS_STRING "UCS-2"
#else
#define UCS_STRING "UCS-4"
#endif

namespace Gwk
{
    namespace Input
    {
        class SDL13
        {
        public:

            SDL13()
            {
                m_canvas = nullptr;
            }

            void Initialize(Gwk::Controls::Canvas* c)
            {
                m_canvas = c;
            }

            bool ProcessEvent(SDL_Event* Event)
            {
                if (!m_canvas)
                    return false;

                switch (Event->type)
                {
                case SDL_KEYUP:
                case SDL_KEYDOWN:
                    {
                        SDL_KeyboardEvent* E = &Event->key;
                        int iKey = -1;
                        SDL_scancode scancode = E->keysym.scancode;

                        switch (scancode)
                        {
                        case SDL_SCANCODE_RETURN:
                            iKey = Gwk::Key::Return;
                            break;

                        case SDL_SCANCODE_BACKSPACE:
                            iKey = Gwk::Key::Backspace;
                            break;

                        case SDL_SCANCODE_DELETE:
                            iKey = Gwk::Key::Delete;
                            break;

                        case SDL_SCANCODE_LEFT:
                            iKey = Gwk::Key::Left;
                            break;

                        case SDL_SCANCODE_RIGHT:
                            iKey = Gwk::Key::Right;
                            break;

                        case SDL_SCANCODE_LSHIFT:
                            iKey = Gwk::Key::Shift;
                            break;

                        case SDL_SCANCODE_RSHIFT:
                            iKey = Gwk::Key::Shift;
                            break;

                        case SDL_SCANCODE_TAB:
                            iKey = Gwk::Key::Tab;
                            break;

                        case SDL_SCANCODE_SPACE:
                            iKey = Gwk::Key::Space;
                            break;

                        case SDL_SCANCODE_HOME:
                            iKey = Gwk::Key::Home;
                            break;

                        case SDL_SCANCODE_END:
                            iKey = Gwk::Key::End;
                            break;

                        case SDL_SCANCODE_LCTRL:
                            iKey = Gwk::Key::Control;
                            break;

                        case SDL_SCANCODE_RCTRL:
                            iKey = Gwk::Key::Control;
                            break;

                        case SDL_SCANCODE_UP:
                            iKey = Gwk::Key::Up;
                            break;

                        case SDL_SCANCODE_DOWN:
                            iKey = Gwk::Key::Down;
                            break;

                        case SDL_SCANCODE_ESCAPE:
                            iKey = Gwk::Key::Escape;
                            break;

                        case SDL_SCANCODE_LALT:
                            iKey = Gwk::Key::Alt;
                            break;

                        case SDL_SCANCODE_RALT:
                            iKey = Gwk::Key::Alt;
                            break;

                        default:
                            return false;
                        }

                        return m_canvas->InputModifierKey(iKey, E->state);
                    }

                case SDL_TEXTINPUT:
                    {
                        SDL_TextInputEvent* E = &Event->text;
                        wchar_t* widechar = (wchar_t*)SDL_iconv_string(UCS_STRING, "UTF-8", E->text, SDL_strlen(
                                                                           E->text)+1);
                        bool ret = m_canvas->InputCharacter(*widechar);
                        SDL_free(widechar);
                        return ret;
                    }

                case SDL_MOUSEMOTION:
                    {
                        SDL_MouseMotionEvent* E = &Event->motion;
                        return m_canvas->InputMouseMoved(E->x, E->y, E->xrel, E->yrel);
                    }

                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    {
                        SDL_MouseButtonEvent* E = &Event->button;
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

                        return m_canvas->InputMouseButton(Button, E->state);
                    }

                case SDL_MOUSEWHEEL:
                    {
                        SDL_MouseWheelEvent* E = &Event->wheel;
                        return m_canvas->InputMouseWheel(E->y);
                    }

                default:
                    return false;
                }
            }

        protected:

            Gwk::Controls::Canvas* m_canvas;

        };


    }
}
#endif // ifndef GWK_INPUT_SDL13_H
