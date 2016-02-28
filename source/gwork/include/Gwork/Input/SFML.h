/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUT_SFML_H
#define GWK_INPUT_SFML_H

#include <Gwork/InputHandler.h>
#include <Gwork/Gwork.h>
#include <Gwork/Controls/Canvas.h>

#include <SFML/Window/Event.hpp>

namespace Gwk
{
    namespace Input
    {
        class SFML
        {
        public:

            SFML()
            {
                m_Canvas = NULL;
                m_MouseX = 0;
                m_MouseY = 0;
            }

            void Initialize(Gwk::Controls::Canvas* c)
            {
                m_Canvas = c;
            }

            unsigned char TranslateKeyCode(int iKeyCode)
            {
                switch (iKeyCode)
                {
#if SFML_VERSION_MAJOR == 2

                case sf::Keyboard::BackSpace:
                    return Gwk::Key::Backspace;

                case sf::Keyboard::Return:
                    return Gwk::Key::Return;

                case sf::Keyboard::Escape:
                    return Gwk::Key::Escape;

                case sf::Keyboard::Tab:
                    return Gwk::Key::Tab;

                case sf::Keyboard::Space:
                    return Gwk::Key::Space;

                case sf::Keyboard::Up:
                    return Gwk::Key::Up;

                case sf::Keyboard::Down:
                    return Gwk::Key::Down;

                case sf::Keyboard::Left:
                    return Gwk::Key::Left;

                case sf::Keyboard::Right:
                    return Gwk::Key::Right;

                case sf::Keyboard::Home:
                    return Gwk::Key::Home;

                case sf::Keyboard::End:
                    return Gwk::Key::End;

                case sf::Keyboard::Delete:
                    return Gwk::Key::Delete;

                case sf::Keyboard::LControl:
                    return Gwk::Key::Control;

                case sf::Keyboard::LAlt:
                    return Gwk::Key::Alt;

                case sf::Keyboard::LShift:
                    return Gwk::Key::Shift;

                case sf::Keyboard::RControl:
                    return Gwk::Key::Control;

                case sf::Keyboard::RAlt:
                    return Gwk::Key::Alt;

                case sf::Keyboard::RShift:
                    return Gwk::Key::Shift;
#else // if SFML_VERSION_MAJOR == 2

                case sf::Key::Back:
                    return Gwk::Key::Backspace;

                case sf::Key::Return:
                    return Gwk::Key::Return;

                case sf::Key::Escape:
                    return Gwk::Key::Escape;

                case sf::Key::Tab:
                    return Gwk::Key::Tab;

                case sf::Key::Space:
                    return Gwk::Key::Space;

                case sf::Key::Up:
                    return Gwk::Key::Up;

                case sf::Key::Down:
                    return Gwk::Key::Down;

                case sf::Key::Left:
                    return Gwk::Key::Left;

                case sf::Key::Right:
                    return Gwk::Key::Right;

                case sf::Key::Home:
                    return Gwk::Key::Home;

                case sf::Key::End:
                    return Gwk::Key::End;

                case sf::Key::Delete:
                    return Gwk::Key::Delete;

                case sf::Key::LControl:
                    return Gwk::Key::Control;

                case sf::Key::LAlt:
                    return Gwk::Key::Alt;

                case sf::Key::LShift:
                    return Gwk::Key::Shift;

                case sf::Key::RControl:
                    return Gwk::Key::Control;

                case sf::Key::RAlt:
                    return Gwk::Key::Alt;

                case sf::Key::RShift:
                    return Gwk::Key::Shift;
#endif // if SFML_VERSION_MAJOR == 2
                }

                return Gwk::Key::Invalid;
            }

            bool ProcessMessage(sf::Event& event)
            {
                if (!m_Canvas)
                    return false;

#if SFML_VERSION_MAJOR == 2

                switch (event.type)
#else

                switch (event.Type)
#endif
                {
                case sf::Event::MouseMoved:
                    {
#if SFML_VERSION_MAJOR == 2
                        int dx = event.mouseMove.x-m_MouseX;
                        int dy = event.mouseMove.y-m_MouseY;
                        m_MouseX = event.mouseMove.x;
                        m_MouseY = event.mouseMove.y;
#else
                        int dx = event.MouseMove.X-m_MouseX;
                        int dy = event.MouseMove.Y-m_MouseY;
                        m_MouseX = event.MouseMove.X;
                        m_MouseY = event.MouseMove.Y;
#endif
                        return m_Canvas->InputMouseMoved(m_MouseX, m_MouseY, dx, dy);
                    }

                case sf::Event::MouseButtonPressed:
                case sf::Event::MouseButtonReleased:
#if SFML_VERSION_MAJOR == 2
                    return m_Canvas->InputMouseButton(event.mouseButton.button,
                                                      event.type ==
                                                      sf::Event::MouseButtonPressed);
#else
                    return m_Canvas->InputMouseButton(event.MouseButton.Button,
                                                      event.Type ==
                                                      sf::Event::MouseButtonPressed);
#endif

                case sf::Event::MouseWheelMoved:
#if SFML_VERSION_MAJOR == 2
                    return m_Canvas->InputMouseWheel(event.mouseWheel.delta*60);
#else
                    return m_Canvas->InputMouseWheel(event.MouseWheel.Delta*60);
#endif

                case sf::Event::TextEntered:
#if SFML_VERSION_MAJOR == 2
                    return m_Canvas->InputCharacter(event.text.unicode);
#else
                    return m_Canvas->InputCharacter(event.Text.Unicode);
#endif

                case sf::Event::KeyPressed:
                case sf::Event::KeyReleased:
                    {
#if SFML_VERSION_MAJOR == 2
                        bool bPressed = (event.type == sf::Event::KeyPressed);
                        char keyCode = event.key.code;
                        bool control = event.key.control;
#else
                        bool bPressed = (event.Type == sf::Event::KeyPressed);
                        char keyCode = event.Key.Code;
                        bool control = event.Key.Control;
#endif

                        if (control && bPressed && keyCode >= 'a' && keyCode <= 'z')
                            return m_Canvas->InputCharacter(keyCode);

                        unsigned char iKey = TranslateKeyCode(keyCode);
                        return m_Canvas->InputKey(iKey, bPressed);
                    }
                        
                default:
                    ;
                }

                return false;
            }

        protected:

            Gwk::Controls::Canvas* m_Canvas;
            int m_MouseX;
            int m_MouseY;

        };


    }
}
#endif // ifndef GWK_INPUT_SFML_H
