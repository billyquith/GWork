/*! 
 *  \file Utility.h 
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_UTILITY_H
#define GWK_UTILITY_H

#include <Gwork/PlatformTypes.h>

#include <sstream>
#include <vector>
#include <stdarg.h>
#include <algorithm>

// Some platforms (e.g. Windows) define these. We want std::min/max.
#ifdef min
#   undef min
#endif
#ifdef max
#   undef max
#endif

namespace Gwk
{
    //! Get the number of items in a static array.
    //! @param A : The array.
#define GWK_ARRAY_COUNT(A) (sizeof(A)/sizeof(A[0]))

    namespace Utility
    {
        GWK_EXPORT int vsnprintf(char* _str, size_t _count, const char* _format, va_list _argList);
        GWK_EXPORT int vsnwprintf(wchar_t* _str, size_t _count, const wchar_t* _format, va_list _argList);
        GWK_EXPORT int snprintf(char* _str, size_t _count, const char* _format, ...);
        GWK_EXPORT int swnprintf(wchar_t* _out, size_t _count, const wchar_t* _format, ...);

        GWK_EXPORT void PrintfVargs(String& _out, const char* _format, va_list _argList);
        GWK_EXPORT void Printf(String& _out, const char* _format, ...);
        GWK_EXPORT String Format(const char* fmt, ...);

        GWK_EXPORT std::wstring Widen(const String &nstr);
        GWK_EXPORT String Narrow(const std::wstring &wstr);

        template <typename TYPE>
        inline String ToString(const TYPE& object)
        {
            std::ostringstream os;
            os << object;
            return os.str();
        }

        GWK_EXPORT void Replace(String& str, const String& strFind, const String& strReplace);


        template<class T, class U>
        static inline std::shared_ptr<T> dynamic_pointer_cast(const std::shared_ptr<U>& r) noexcept
        {
            if (auto p = dynamic_cast<typename std::shared_ptr<T>::element_type*>(r.get())) 
                return std::shared_ptr<T>(r, p);
            else
                return std::shared_ptr<T>();
        }
        
        namespace Strings
        {
            typedef std::vector<Gwk::String> List;

            GWK_EXPORT void Split(const Gwk::String& str, const Gwk::String& seperator,
                                   Strings::List& outbits, bool bLeaveSeperators = false);
            GWK_EXPORT bool Wildcard(const Gwk::String& strWildcard,
                                      const Gwk::String& strHaystack);

            GWK_EXPORT void ToUpper(Gwk::String& str);
            GWK_EXPORT void Strip(Gwk::String& str, const Gwk::String& chars);

            template <typename T>
            T TrimLeft(const T& str, const T& strChars)
            {
                T outstr = str;
                outstr.erase(0, outstr.find_first_not_of(strChars));
                return outstr;
            }

            //! \brief Widen a UTF8 code to a wide encoding.
            //!
            //! Source: https://stackoverflow.com/a/148766/3233
            //
            static inline wchar_t utf8_to_wchart(char*& in)
            {
#if defined(__clang__) && defined(__apple_build_version__) && __apple_build_version__ >= 8
                // thread_local not supported on Xcode versions below 8
                static unsigned int next = 0x10000;
#else
                thread_local static unsigned int next = 0x10000;
#endif
                if (next != 0x10000)
                {
                    wchar_t ret = static_cast<wchar_t>(next);
                    next = 0x10000;
                    return ret;
                }
                unsigned int codepoint;

                while (*in != 0)
                {
                    unsigned char ch = static_cast<unsigned char>(*in);
                    if (ch <= 0x7f)
                        codepoint = ch;
                    else if (ch <= 0xbf)
                        codepoint = (codepoint << 6) | (ch & 0x3f);
                    else if (ch <= 0xdf)
                        codepoint = ch & 0x1f;
                    else if (ch <= 0xef)
                        codepoint = ch & 0x0f;
                    else
                        codepoint = ch & 0x07;
                    ++in;
                    if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
                    {
                        if (sizeof(wchar_t) > 2)
                            return static_cast<wchar_t>(codepoint);
                        else if (codepoint > 0xffff)
                        {
                            next = static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff));
                            return static_cast<wchar_t>(0xd800 + (codepoint >> 10));
                        }
                        else if (codepoint < 0xd800 || codepoint >= 0xe000)
                            return static_cast<wchar_t>(codepoint);
                    }
                }
                return 0;
            }

            namespace To
            {
                GWK_EXPORT bool  Bool(const Gwk::String& str);
                GWK_EXPORT int   Int(const Gwk::String& str);
                GWK_EXPORT float Float(const Gwk::String& str);
                GWK_EXPORT float Float(const Gwk::String& str);
                GWK_EXPORT bool  Floats(const Gwk::String& str, float* f, size_t iCount);
            }
        }
    }

    template <typename T>
    inline T Clamp(T current, T vmin, T vmax)
    {
        return current < vmin ? vmin : (current > vmax ? vmax : current);
    }
 }

#endif // ifndef GWK_UTILITY_H
