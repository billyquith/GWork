/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_UITYPES_H
#define GWK_UITYPES_H

#ifdef _MSC_VER
#   pragma warning( disable : 4244 )
#   pragma warning( disable : 4251 )
#endif

#include <Gwork/PlatformTypes.h>

namespace Gwk
{
    namespace Controls
    {
        class Base;
        class Canvas;
    }
    
    struct GWK_EXPORT Margin
    {
        Margin(int left_ = 0, int top_ = 0, int right_ = 0, int bottom_ = 0)
        :   top(top_)
        ,   bottom(bottom_)
        ,   left(left_)
        ,   right(right_)
        {}

        Margin operator + (const Margin& margin) const
        {
            Margin m;
            m.top       = top + margin.top;
            m.bottom    = bottom + margin.bottom;
            m.left      = left + margin.left;
            m.right     = right + margin.right;
            return m;
        }

        int top, bottom, left, right;
    };

    typedef Margin Padding;

    struct GWK_EXPORT HSV
    {
        float h;
        float s;
        float v;
    };

    namespace DragAndDrop
    {
        struct GWK_EXPORT Package
        {
            Package()
            {
                userdata = nullptr;
                draggable = false;
                drawcontrol = nullptr;
                holdoffset = Gwk::Point(0, 0);
            }

            String  name;
            void*   userdata;
            bool    draggable;

            Gwk::Controls::Base*   drawcontrol;
            Gwk::Point             holdoffset;
        };


    }

    enum class Position
    {
        None        = 0,
        Left        = (1<<1),
        Right       = (1<<2),
        Top         = (1<<3),
        Bottom      = (1<<4),
        CenterV     = (1<<5),
        CenterH     = (1<<6),
        Fill        = (1<<7),
        Center      = CenterV | CenterH
    };
    
    inline Position operator | (Position a, Position b)
    {
        return Position(static_cast<int>(a) | static_cast<int>(b));
    }

    inline bool operator & (Position a, Position b)
    {
        return (static_cast<int>(a) & static_cast<int>(b)) != 0;
    }

} // namespace Gwk
    
#endif // ifndef GWK_UITYPES_H
