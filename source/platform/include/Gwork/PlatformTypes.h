/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once

#ifndef GWK_PLATFORMTYPES_H
#define GWK_PLATFORMTYPES_H

#ifdef _MSC_VER
#   pragma warning( disable : 4244 )
#   pragma warning( disable : 4251 )
#endif

#include <Gwork/Exports.h>
#include <string>
#include <list>

namespace Gwk
{
    namespace Controls
    {
        class Base;
        class Canvas;
    }

    //
    // All strings are UTF-8 for Unicode. This is backwards compatible with ASCII.
    // See: http://www.utf8everywhere.org
    //
    typedef std::string     String;
    typedef char            UnicodeChar;

    namespace CursorType
    {
        enum
        {
            Normal   = 0,
            Beam     = 1,
            SizeNS   = 2,
            SizeWE   = 3,
            SizeNWSE = 4,
            SizeNESW = 5,
            SizeAll  = 6,
            No       = 7,
            Wait     = 8,
            Finger   = 9,
            Cursor_Count = 10
        };
    }
    
    struct GWK_EXPORT Rect
    {
        Rect(int x_ = 0, int y_ = 0, int w_ = 0, int h_ = 0)
        :   x(x_), y(y_), w(w_), h(h_)
        {}
        
        bool operator == (const Rect &other) const
        {
            return x == other.x && y == other.y && w == other.w && h == other.h;
        }

        Rect operator + (const Rect& rct) const
        {
            Rect m;
            m.x = x+rct.x;
            m.y = y+rct.y;
            m.w = w+rct.w;
            m.h = h+rct.h;
            return m;
        }

        int x, y, w, h;
    };

    struct GWK_EXPORT Point
    {
        Point(int x_=0, int y_=0)
        :   x(x_), y(y_)
        {}

        void operator += (const Point& p)
        {
            x += p.x;
            y += p.y;
        }

        Point operator + (const Point& p) const
        {
            return Point(x + p.x, p.y + y);
        }

        void operator -= (const Point& p)
        {
            x -= p.x;
            y -= p.y;
        }

        Point operator - (const Point& p) const
        {
            return Point(x - p.x, y - p.y);
        }

        int x, y;
    };

    struct GWK_EXPORT Color
    {
        Color(unsigned char r_=255,
              unsigned char g_=255,
              unsigned char b_=255,
              unsigned char a_=255)
        :   r(r_)
        ,   g(g_)
        ,   b(b_)
        ,   a(a_)
        {}

        void operator = (Color c)
        {
            r = c.r;
            g = c.g;
            b = c.b;
            a = c.a;
        }

        void operator += (Color c)
        {
            r += c.r;
            g += c.g;
            b += c.b;
            a += c.a;
        }

        void operator -= (Color c)
        {
            r -= c.r;
            g -= c.g;
            b -= c.b;
            a -= c.a;
        }

        void operator *= (float f)
        {
            *this = *this * f;
        }

        Color operator * (float f) const
        {
            return Color(f*r, f*g, f*b, f*a);
        }

        Color operator - (Color c) const
        {
            return Color(r - c.r, g - c.g, b - c.b, a - c.a);
        }

        Color operator + (Color c) const
        {
            return Color(r + c.r, g + c.g, b + c.b, a + c.a);
        }

        bool operator == (const Color& c) const
        {
            return c.r==r && c.g==g && c.b==b && c.a==a;
        }

        unsigned char r, g, b, a;
    };

    struct Font
    {
        typedef std::list<Font*> List;

        Font()
        :   facename("Arial")
        ,   size(10)
        ,   bold(false)
        ,   data(NULL)
        ,   realsize(0)
        {
        }

        String facename;
        float size;
        bool bold;

        /// This should be set by the renderer
        /// if it tries to use a font where it's NULL.
        void*   data;

        /// This is the real font size, after it's been scaled by Render->Scale()
        float realsize;
    };

    struct Texture
    {
        typedef std::list<Texture*> List;

        String  name;
        void*   data;
        bool    failed;
        int     width;
        int     height;
        
        bool    readable;
        void*   surface;

        Texture()
        :   data(NULL)
        ,   failed(false)
        ,   width(4)
        ,   height(4)
        ,   readable(false)
        ,   surface(NULL)
        {
        }
    };
    
}
#endif // ifndef GWK_PLATFORMTYPES_H
