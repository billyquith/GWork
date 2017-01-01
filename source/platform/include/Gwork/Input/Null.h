/*
 *  Gwork
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_INPUT_NULL_H
#define GWK_INPUT_NULL_H

#include <Gwork/InputEventListener.h>

namespace Gwk
{
    namespace Input
    {
        class Null
        {
        public:

            Null()
            :   m_eventListener(nullptr)
            // ,   m_mouseX(0)
            // ,   m_mouseY(0)
            {
            }

            void Initialize(IInputEventListener* el)
            {
                m_eventListener = el;
            }

            unsigned char TranslateKeyCode(int iKeyCode)
            {
                return Gwk::Key::Invalid;
            }

            // bool ProcessMessage()
            // {
            //     return false;
            // }

        protected:

            IInputEventListener *m_eventListener;
            // int m_mouseX;
            // int m_mouseY;

        };
    }
}

#endif // GWK_INPUT_NULL_H
