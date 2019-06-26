/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
#include <algorithm>
#include <limits>

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
        Fixed       = (1<<8),
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

    enum class Dim
    {
        X,
        Y
    };

    enum class SizeFlag
    {
        Fixed=0, //Will not change size
        Expand=1, //Will only expand
        Shrink=2, //Will only shrink
        Elastic=3, //Expands or shrinks as needed
        Bloat=4 //Will take all available space
    };

    inline std::string SizeFlagName(SizeFlag sizeFlag)
    {
        switch(sizeFlag)
        {
        case SizeFlag::Fixed:
            return "Fixed";
            break;
        case SizeFlag::Expand:
            return "Expand";
            break;
        case SizeFlag::Shrink:
            return "Shrink";
            break;
        case SizeFlag::Elastic:
            return "Elastic";
            break;
        case SizeFlag::Bloat:
            return "Bloat";
            break;
        default:
            return "Unknown";
            break;
        }
        return "Unknown";
    }

    class SizeFlags
    {
    public:
        SizeFlags():horizontal(SizeFlag::Expand), vertical(SizeFlag::Expand) {}
        SizeFlags(SizeFlag horizontal, SizeFlag vertical):horizontal(horizontal), vertical(vertical) {}

        union
        {
            struct{
                SizeFlag horizontal;
                SizeFlag vertical;
            };
            SizeFlag value[2];
        };

    };

    inline SizeFlag Merge(SizeFlag flag1, SizeFlag flag2)
    {
        typedef std::underlying_type<SizeFlag>::type Type;
        Type _flag1=static_cast<Type>(flag1);
        Type _flag2=static_cast<Type>(flag2);

        if(_flag1+_flag2==3)
            return SizeFlag::Elastic;
        else
            return static_cast<SizeFlag>(std::max(_flag1, _flag2));
    }

    

    inline int AddWithOverflow(int value1, int value2)
    {
        if((value2>0) && (value1>std::numeric_limits<int>::max()-value2))
            return std::numeric_limits<int>::max();
        if((value2<0) && (value1<std::numeric_limits<int>::min()-value2))
            return 0;
        return value1+value2;
    }
} // namespace Gwk

#endif // ifndef GWK_UITYPES_H
