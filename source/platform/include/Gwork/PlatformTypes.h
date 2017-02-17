/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once

#ifndef GWK_PLATFORMTYPES_H
#define GWK_PLATFORMTYPES_H

#ifdef _MSC_VER
#   pragma warning( disable : 4244 )
#   pragma warning( disable : 4251 )
#endif

#include <Gwork/Config.h>
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
    typedef char            UnicodeChar;    // TODO - deal with multibyte UTF8 chars.

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
    
    struct GWK_EXPORT Point
    {
        Point(int x_=0, int y_=0)
        :   x(x_), y(y_)
        {}
        
        void set(int x_, int y_)
        {
            x = x_, y = y_;
        }
        
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

    struct GWK_EXPORT Rect
    {
        Rect(int x_ = 0, int y_ = 0, int w_ = 0, int h_ = 0)
        :   x(x_), y(y_), w(w_), h(h_)
        {}
        
        Rect(const Point& o, const Point& sz)
        :   x(o.x), y(o.y), w(sz.x), h(sz.y)
        {}
        
        bool operator == (const Rect &other) const
        {
            return x == other.x && y == other.y && w == other.w && h == other.h;
        }

        Rect operator + (const Rect& rct) const
        {
            const Rect m(x + rct.x, y + rct.y, w + rct.w, h + rct.h);
            return m;
        }
        
        Point GetSize() const { return Point(w,h); }

        int x, y, w, h;
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

        Color(const Color& c)
        {
            r = c.r;
            g = c.g;
            b = c.b;
            a = c.a;
        }

        void operator = (Color c)
        {
            r = c.r;
            g = c.g;
            b = c.b;
            a = c.a;
        }

        void operator += (Color c)
        {
            r = static_cast<unsigned char>(r + c.r);
            g = static_cast<unsigned char>(g + c.g);
            b = static_cast<unsigned char>(b + c.b);
            a = static_cast<unsigned char>(a + c.a);
        }

        void operator -= (Color c)
        {
            r = static_cast<unsigned char>(r - c.r);
            g = static_cast<unsigned char>(g - c.g);
            b = static_cast<unsigned char>(b - c.b);
            a = static_cast<unsigned char>(a - c.a);
        }

        void operator *= (float f)
        {
            *this = *this * f;
        }

        Color operator * (float f) const
        {
            return Color(
                    static_cast<unsigned char>(f*r),
                    static_cast<unsigned char>(f*g),
                    static_cast<unsigned char>(f*b),
                    static_cast<unsigned char>(f*a)
                    );
        }

        Color operator - (Color c) const
        {
            return Color(
                    static_cast<unsigned char>(r - c.r),
                    static_cast<unsigned char>(g - c.g),
                    static_cast<unsigned char>(b - c.b),
                    static_cast<unsigned char>(a - c.a)
                    );
        }

        Color operator + (Color c) const
        {
            return Color(
                    static_cast<unsigned char>(r + c.r),
                    static_cast<unsigned char>(g + c.g),
                    static_cast<unsigned char>(b + c.b),
                    static_cast<unsigned char>(a + c.a)
                    );
        }

        bool operator == (const Color& c) const
        {
            return c.r==r && c.g==g && c.b==b && c.a==a;
        }

        unsigned char r, g, b, a;
    };

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
    
    struct Font
    {
        typedef std::list<Font*> List;

        Font()
        :   facename("Arial")
        ,   size(10)
        ,   bold(false)
        ,   data(nullptr)
        ,   realsize(0)
        ,   render_data(nullptr)
        {
        }

        String facename;
        float size;
        bool bold;

        void *data;             // Font data, set by renderer

        // This is the real font size, after it's been scaled by Render->Scale()
        float realsize;
        
        void *render_data;      // optional renderer data
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
        :   data(nullptr)
        ,   failed(false)
        ,   width(4)
        ,   height(4)
        ,   readable(false)
        ,   surface(nullptr)
        {
        }
    };
    
}
#endif // ifndef GWK_PLATFORMTYPES_H
