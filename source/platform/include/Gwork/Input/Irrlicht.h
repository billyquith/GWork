/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */
#pragma once
#include <irrlicht.h>
#include <Gwork/InputEventListener.h>

namespace Gwk
{
    namespace Input
    {
        class Irrlicht
        {
        public:
            Irrlicht() :
                m_eventListener(nullptr)
            {
                MouseX = 0;
                MouseY = 0;
            }

            ~Irrlicht()
            {}

            void Initialize(IInputEventListener* el)
            {
                m_eventListener = el;
            }

            bool HandleEvents(const irr::SEvent& IrrEvent)
            {
                switch (IrrEvent.EventType)
                {
                case irr::EET_MOUSE_INPUT_EVENT:
                    switch (IrrEvent.MouseInput.Event)
                    {
                    case irr::EMIE_MOUSE_MOVED:
                    {
                        int x = IrrEvent.MouseInput.X;
                        int y = IrrEvent.MouseInput.Y;
                        int dx = x - MouseX;
                        int dy = y - MouseY;
                        MouseX = x;
                        MouseY = y;
                        return m_eventListener->InputMouseMoved(x, y, dx, dy);
                    }

                    case irr::EMIE_MOUSE_WHEEL:
                        return m_eventListener->InputMouseWheel(IrrEvent.MouseInput.Wheel);

                    case irr::EMIE_LMOUSE_PRESSED_DOWN:
                        return m_eventListener->InputMouseButton(0, true);

                    case irr::EMIE_LMOUSE_LEFT_UP:
                        return m_eventListener->InputMouseButton(0, false);

                    case irr::EMIE_RMOUSE_PRESSED_DOWN:
                        return m_eventListener->InputMouseButton(1, true);

                    case irr::EMIE_RMOUSE_LEFT_UP:
                        return m_eventListener->InputMouseButton(1, false);

                    case irr::EMIE_MMOUSE_PRESSED_DOWN:
                        return m_eventListener->InputMouseButton(2, true);

                    case irr::EMIE_MMOUSE_LEFT_UP:
                        return m_eventListener->InputMouseButton(2, false);

                    default:
                        return false;
                    }

                case irr::EET_KEY_INPUT_EVENT:
                    switch (IrrEvent.KeyInput.Key)
                    {
                    case irr::KEY_SHIFT:
                        return m_eventListener->InputModifierKey(Gwk::Key::Shift, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_RETURN:
                        return m_eventListener->InputModifierKey(Gwk::Key::Return, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_BACK:
                        return m_eventListener->InputModifierKey(Gwk::Key::Backspace, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_DELETE:
                        return m_eventListener->InputModifierKey(Gwk::Key::Delete, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_LEFT:
                        return m_eventListener->InputModifierKey(Gwk::Key::Left, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_RIGHT:
                        return m_eventListener->InputModifierKey(Gwk::Key::Right, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_TAB:
                        return m_eventListener->InputModifierKey(Gwk::Key::Tab, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_SPACE:
                        return m_eventListener->InputModifierKey(Gwk::Key::Space, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_HOME:
                        return m_eventListener->InputModifierKey(Gwk::Key::Home, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_END:
                        return m_eventListener->InputModifierKey(Gwk::Key::End, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_CONTROL:
                        return m_eventListener->InputModifierKey(Gwk::Key::Control, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_UP:
                        return m_eventListener->InputModifierKey(Gwk::Key::Up, IrrEvent.KeyInput.PressedDown);

                    case irr::KEY_DOWN:
                        return m_eventListener->InputModifierKey(Gwk::Key::Down, IrrEvent.KeyInput.PressedDown);

                    default:
                        return m_eventListener->InputCharacter((Gwk::UnicodeChar)IrrEvent.KeyInput.Char);
                    }

                default:
                    return false;
                }
            }

        protected:
            IInputEventListener * m_eventListener;
            irr::s32 MouseX;
            irr::s32 MouseY;
        };
    }
}
