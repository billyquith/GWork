/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#pragma once
#ifndef GWEN_UTILITY_H
#define GWEN_UTILITY_H

#include <sstream>
#include <vector>
#include "Gwen/Structures.h"

#include <stdarg.h>

namespace Gwen
{
    namespace Utility
    {
     
        GWEN_EXPORT int vsnprintf(char* _str, size_t _count, const char* _format, va_list _argList);
        GWEN_EXPORT int vsnwprintf(wchar_t* _str, size_t _count, const wchar_t* _format, va_list _argList);
        GWEN_EXPORT int snprintf(char* _str, size_t _count, const char* _format, ...);
        GWEN_EXPORT int swnprintf(wchar_t* _out, size_t _count, const wchar_t* _format, ...);
        GWEN_EXPORT void PrintfVargs(String& _out, const char* _format, va_list _argList);
        GWEN_EXPORT void Printf(String& _out, const char* _format, ...);
        GWEN_EXPORT String Format(const char* fmt, ...);

        GWEN_EXPORT std::wstring Widen(const String &nstr);
        GWEN_EXPORT String Narrow(const std::wstring &wstr);

        template <typename TYPE>
        inline String ToString(const TYPE& object)
        {
            std::ostringstream os;
            os << object;
            return os.str();
        }

        GWEN_EXPORT void Replace(String& str, const String& strFind, const String& strReplace);

        namespace Strings
        {
            typedef std::vector<Gwen::String> List;

            GWEN_EXPORT void Split(const Gwen::String& str, const Gwen::String& seperator,
                                   Strings::List& outbits, bool bLeaveSeperators = false);
            GWEN_EXPORT bool Wildcard(const Gwen::String& strWildcard,
                                      const Gwen::String& strHaystack);

            GWEN_EXPORT void ToUpper(Gwen::String& str);
            GWEN_EXPORT void Strip(Gwen::String& str, const Gwen::String& chars);

            template <typename T>
            T TrimLeft(const T& str, const T& strChars)
            {
                T outstr = str;
                outstr.erase(0, outstr.find_first_not_of(strChars));
                return outstr;
            }

            namespace To
            {
                GWEN_EXPORT bool  Bool(const Gwen::String& str);
                GWEN_EXPORT int   Int(const Gwen::String& str);
                GWEN_EXPORT float Float(const Gwen::String& str);
                GWEN_EXPORT float Float(const Gwen::String& str);
                GWEN_EXPORT bool  Floats(const Gwen::String& str, float* f, size_t iCount);
            }
        }
        
        Gwen::Rect ClampRectToRect(Gwen::Rect inside, Gwen::Rect outside, bool clampSize = false);

    }
}

#endif // ifndef GWEN_UTILITY_H
