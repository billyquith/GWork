/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
#include <functional>
#include <string>
#include <list>
#include <memory>

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
        :   x(x_) , y(y_)
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

        int Left() const { return x; }
        int Right() const { return x + w; }
        int Top() const { return y; }
        int Bottom() const { return y + h; }

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
        :   r(r_), g(g_), b(b_), a(a_)
        {}

        Color(const Color& c)
        {
            rgba = c.rgba;
        }

        Color(Color&& c)
        {
            rgba = c.rgba;
        }

        void operator = (const Color& c)
        {
            rgba = c.rgba;
        }

        void operator = (Color&& c)
        {
            rgba = c.rgba;
        }

        void operator += (const Color& c)
        {
            r = static_cast<unsigned char>(r + c.r);
            g = static_cast<unsigned char>(g + c.g);
            b = static_cast<unsigned char>(b + c.b);
            a = static_cast<unsigned char>(a + c.a);
        }

        void operator -= (const Color& c)
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

        Color operator - (const Color& c) const
        {
            return Color(
                    static_cast<unsigned char>(r - c.r),
                    static_cast<unsigned char>(g - c.g),
                    static_cast<unsigned char>(b - c.b),
                    static_cast<unsigned char>(a - c.a)
                    );
        }

        Color operator + (const Color& c) const
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

        union {
            uint32_t rgba;
            struct { unsigned char r, g, b, a; };
        };
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

        enum class Status
        {
            Unloaded,               //!< As yet, unloaded.
            Loaded,                 //!< Loaded successful.
            ErrorFileNotFound,      //!< File requested was not found.
            ErrorBadData,           //!< Resource was bad data.
            MaxStatus
        };

        Font()
        :   facename("?")
        ,   size(10)
        ,   bold(false)
        {}

        Font(const Font&) = default;
        Font& operator=(const Font&) = default;

        inline bool operator==(const Font& rhs) const
        {
            return facename == rhs.facename && size == rhs.size && bold == rhs.bold;
        }

        inline bool operator!=(const Font& rhs) const
        {
            return  bold != rhs.bold || size != rhs.size || facename != rhs.facename;
        }

        String facename;
        float size;
        bool bold;
    };

    struct Texture
    {
        typedef std::list<Texture*> List;

        //! Status of load operation.
        enum class Status
        {
            Unloaded,               //!< As yet, unloaded.
            Loaded,                 //!< Loaded successful.
            ErrorFileNotFound,      //!< File requested was not found.
            ErrorBadData,           //!< Resource was bad data.
            MaxStatus
        };

        Texture()
            : readable(false)
        {}

        Texture(const Texture&) = default;
        Texture& operator=(const Texture&) = default;

        inline bool operator==(const Texture& rhs) const { return name == rhs.name; }
        inline bool operator!=(const Texture& rhs) const { return name != rhs.name; }

        String  name;
        bool readable;
    };

    struct TextureData
    {
        TextureData()
            :   width(0.f), height(0.f)
        {}

        float width;
        float height;
        bool readable;
    };


    //! Base class for resource path calculation.
    //!
    //! Can be implemented by different platforms for different file layouts.
    class ResourcePaths
    {
    public:
        //! Resource type.
        enum class Type
        {
            Font,           //!< Font resource.
            Texture,        //!< Texture resource.
            Other,          //!< Other type of resource.
            MaxType
        };

        virtual ~ResourcePaths() {}
        virtual String GetPath(Type type, String const& name) = 0;
    };


    //! Interface for resource loaders.
    //!
    //! These are used to load, create and destroy resource needed for Gwork.
    class IResourceLoader
    {
    public:

        //! Types of notification that the loader might want to know about.
        enum class NotificationType
        {
            DeviceLost,             //!< Device lost, perhaps release resources.
            DeviceAcquired,         //!< Device reappeared, perhaps reload resources.
            ReleaseResources,       //!< Release any resources held.
            MaxMessages,
            UserMessageStart = 100
        };

        virtual Gwk::Font::Status LoadFont(const Gwk::Font& font) = 0;
        virtual void FreeFont(const Gwk::Font& font) = 0;

        virtual Gwk::Texture::Status LoadTexture(const Gwk::Texture& texture) = 0;
        virtual void FreeTexture(const Gwk::Texture& texture) = 0;
        virtual Gwk::TextureData GetTextureData(const Gwk::Texture& texture) const = 0;

        //! Notification of certain events. May be platform specific.
        //! Loader can deal with the events accordingly.
        virtual void Notify(NotificationType msg) {}
    };

    template <class T>
    static inline void hash_combine(size_t& seed, const T& v)
    {
        seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
} // namespace Gwk

// Hashers
namespace std
{
    template<> struct hash<Gwk::Font>
    {
        typedef Gwk::Font argument_type;
        typedef std::size_t result_type;


        result_type operator()(argument_type const& f) const noexcept
        {
            result_type res = std::hash<Gwk::String>{}(f.facename);

            Gwk::hash_combine<decltype(f.size)>(res, f.size);
            Gwk::hash_combine<decltype(f.bold)>(res, f.bold);

            return res;
        }
    };
    template<> struct hash<Gwk::Texture>
    {
        typedef Gwk::Texture argument_type;
        typedef std::size_t result_type;


        result_type operator()(argument_type const& f) const noexcept
        {
            return std::hash<decltype(f.name)>{}(f.name);
        }
    };
} // namespace std

#endif // ifndef GWK_PLATFORMTYPES_H
