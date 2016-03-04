/*
 *  Gwork
 *
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */

#pragma once
#ifndef GWK_GWK_H
#define GWK_GWK_H

#include <stdlib.h>
#include <memory.h>
#include <set>
#include <list>
#include <algorithm>

#include <Gwork/Macros.h>
#include <Gwork/Config.h>
#include <Gwork/Exports.h>
#include <Gwork/UiTypes.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Canvas.h>
#include <Gwork/Align.h>

// Enable the hook system (se Hook.h)
#define GWK_HOOKSYSTEM 1

namespace Gwk
{
    namespace Controls
    {
        class Base;
        class Canvas;
    }

    namespace Renderer
    {
        class Base;
    }

    namespace Debug
    {
        void GWK_EXPORT Msg(const wchar_t* str, ...);
        void GWK_EXPORT Msg(const char* str, ...);
        void GWK_EXPORT AssertCheck(bool b, const char* strMsg);
    }


    namespace Colors
    {
        static const Color Black(0, 0, 0, 255);
        static const Color Red(255, 0, 0, 255);
        static const Color Yellow(255, 255, 0, 255);
        static const Color White(255, 255, 255, 255);
        static const Color Blue(0, 0, 255, 255);
        static const Color Green(0, 255, 0, 255);
        static const Color Grey(200, 200, 200, 255);
        static const Color GreyLight(230, 230, 230, 255);
        static const Color GworkPink(255, 65, 199, 255);
    }

    extern GWK_EXPORT Controls::Base*  HoveredControl;
    extern GWK_EXPORT Controls::Base*  KeyboardFocus;
    extern GWK_EXPORT Controls::Base*  MouseFocus;

    namespace Input
    {
        inline void Unfocus()
        {
            if (KeyboardFocus)
                KeyboardFocus->Unfocus();
        }

    }

} // namespace Gwk

#endif // ifndef GWK_GWK_H
