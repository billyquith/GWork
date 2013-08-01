/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#include "Gwen/ToolTip.h"
#include "Gwen/Utility.h"

#include <cstdio>
#include <locale>       // Narrow/widen
#include <codecvt>      // Narrow/widen

using namespace Gwen;

#ifdef _MSC_VER
#   pragma warning(disable:4267)    // conversion from 'size_t' to 'int', possible loss of data
#endif

#ifdef __MINGW32__
#   undef vswprintf
#   define vswprintf _vsnwprintf
#endif

#ifdef _MSC_VER
#   define GWEN_FNULL "NUL"
#   define va_copy(c, s)   (c) = (s)       // MSVC does not implement C99.
#else
#   define GWEN_FNULL "/dev/null"
#endif


std::wstring Gwen::Utility::Widen(const Gwen::String &nstr)
{
    // UTF-8 to UTF-16 (C++11)
    // See: http://en.cppreference.com/w/cpp/locale/codecvt_utf8_utf16
    // See: http://www.cplusplus.com/reference/codecvt/codecvt_utf8_utf16/

    std::wstring_convert< std::codecvt_utf8_utf16<wchar_t>, wchar_t > conversion;
    const std::wstring wstr( conversion.from_bytes( nstr.c_str() ) );

    return wstr;
}

Gwen::String Gwen::Utility::Narrow(const std::wstring &wstr)
{
    // wide to UTF-8 (C++11)
    // See: http://en.cppreference.com/w/cpp/locale/wstring_convert/to_bytes

    std::wstring_convert< std::codecvt_utf8<wchar_t> > conv1;
    Gwen::String u8str = conv1.to_bytes(wstr);

    return u8str;
}

void Gwen::Utility::Replace(String& str, const String& strFind, const String& strReplace)
{
    size_t pos = 0;
    
    while ((pos = str.find(strFind, pos)) != String::npos)
    {
        str.replace(pos, strFind.length(), strReplace);
        pos += strReplace.length();
    }
}

String Gwen::Utility::Format(const char* fmt, ...)
{
    va_list s;
    int len = 0;

    va_start(s, fmt);

    // Determine the length of the resulting string, this method is much faster
    // than looping and reallocating a bigger buffer size.
    {
#ifdef WIN32
        FILE* fnull = NULL;
        fopen_s(&fnull, GWEN_FNULL, "wb");
#else
        FILE* fnull = fopen(GWEN_FNULL, "wb");
#endif
        va_list c;
        va_copy(c, s);
        len = vfprintf(fnull, fmt, c);
        va_end(c);
        fclose(fnull);
    }

    String strOut;

    if (len > 0)
    {
        strOut.resize(len+1);
        va_list c;
        va_copy(c, s);
#ifdef _MSC_VER
        len = vsprintf_s(&strOut[0], strOut.size(), fmt, c);
#else
        len = vsprintf(&strOut[0], fmt, c);
#endif
        va_end(c);
        strOut.resize(len);
    }

    va_end(s);

    return strOut;
}

void Gwen::Utility::Strings::Split(const Gwen::String& str, const Gwen::String& seperator,
                                   Strings::List& outbits, bool bLeave)
{
    size_t iOffset = 0;
    size_t iLength = str.length();
    size_t iSepLen = seperator.length();
    size_t i = str.find(seperator, 0);

    while (i != std::string::npos)
    {
        outbits.push_back(str.substr(iOffset, i-iOffset));
        iOffset = i+iSepLen;
        i = str.find(seperator, iOffset);

        if (bLeave)
            iOffset -= iSepLen;
    }

    outbits.push_back(str.substr(iOffset, iLength-iOffset));
}

int Gwen::Utility::Strings::To::Int(const Gwen::String& str)
{
    if (str == "")
        return 0;

    return atoi(str.c_str());
}

float Gwen::Utility::Strings::To::Float(const Gwen::String& str)
{
    if (str == "")
        return 0.0f;

    return static_cast<float>( atof(str.c_str()) );
}

bool Gwen::Utility::Strings::To::Bool(const Gwen::String& str)
{
    if (str.size() == 0)
        return false;

    if (str[0] == 'T' || str[0] == 't' || str[0] == 'y' || str[0] == 'Y')
        return true;

    if (str[0] == 'F' || str[0] == 'f' || str[0] == 'n' || str[0] == 'N')
        return false;

    if (str[0] == '0')
        return false;

    return true;
}

bool Gwen::Utility::Strings::To::Floats(const Gwen::String& str, float* f, size_t iCount)
{
    Strings::List lst;
    Strings::Split(str, " ", lst);

    if (lst.size() != iCount)
        return false;

    for (size_t i = 0; i < iCount; i++)
    {
        f[i] = Strings::To::Float(lst[i]);
    }

    return true;
}

bool Gwen::Utility::Strings::Wildcard(const String& strWildcard, const String& strHaystack)
{
    const String& W = strWildcard;
    const String& H = strHaystack;

    if (strWildcard == "*")
        return true;

    size_t iPos = W.find("*", 0);

    if (iPos == String::npos)
        return strWildcard == strHaystack;

    // First half matches
    if (iPos > 0 && W.substr(0, iPos) != H.substr(0, iPos))
        return false;

    // Second half matches
    if (iPos != W.length()-1)
    {
        String strEnd = W.substr(iPos+1, W.length());

        if (strEnd != H.substr(H.length()-strEnd.length(), H.length()))
            return false;
    }

    return true;
}

void Gwen::Utility::Strings::ToUpper(Gwen::String& str)
{
    transform(str.begin(), str.end(), str.begin(), toupper);
}

void Gwen::Utility::Strings::Strip(Gwen::String& str, const Gwen::String& chars)
{
    Gwen::String Source = str;
    str = "";

    for (unsigned int i = 0; i < Source.length(); i++)
    {
        if (chars.find(Source[i]) != Gwen::String::npos)
            continue;

        str += Source[i];
    }
}


Gwen::Rect Gwen::Utility::ClampRectToRect(Gwen::Rect inside, Gwen::Rect outside, bool clampSize)
{
    if (inside.x < outside.x)
        inside.x = outside.x;
        
    if (inside.y  < outside.y)
        inside.y = outside.y;

    if (inside.x+inside.w > outside.x+outside.w)
    {
        if (clampSize)
            inside.w = outside.w;
        else
            inside.x = outside.x+outside.w-inside.w;
    }

    if (inside.y+inside.h > outside.y+outside.h)
    {
        if (clampSize)
            inside.h = outside.h;
        else
            inside.y = outside.w+outside.h-inside.h;
    }
    
    return inside;
}



