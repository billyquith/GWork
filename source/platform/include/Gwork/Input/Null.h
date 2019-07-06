/*
 *  Gwork
 *  Copyright (c) 2013-2018 Billy Quith
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
            {}

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
        };
    }
}

#endif // GWK_INPUT_NULL_H
