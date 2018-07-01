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
