/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#include "Gwen/ToolTip.h"
#include "Gwen/Utility.h"

#include <cstdio>

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


Gwen::String Gwen::Utility::UnicodeToString(const UnicodeString& strIn)
{
    if (!strIn.length())
        return "";
    
    const size_t ssz = strIn.length();
    const wchar_t *wstr = strIn.c_str();
    String ret(ssz, (char)0);
    
    // Convert the wide string to a C string, replacing chars we cannot match with "_".
    // Note: This will mangle most non-European Unicode strings as the range of
    //       characters required don't fit in a byte.
    std::use_facet< std::ctype<wchar_t> >(std::locale("")).narrow(wstr, wstr+ssz, '_', &ret[0]);
    
    return ret;
}

Gwen::UnicodeString Gwen::Utility::StringToUnicode(const String& strIn)
{
    if (!strIn.length())
        return L"";
    
    const size_t ssz = strIn.length();
    const char *str = strIn.c_str();    
    UnicodeString ret(ssz, (wchar_t)0);
    
    // Create a wide string, encoding each character of the C string in current locale.
    std::use_facet<std::ctype<wchar_t> >(std::locale()).widen(str, str+ssz, &ret[0] );
    
    return ret;
}

UnicodeString Gwen::Utility::Format(const wchar_t* fmt, ...)
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
        len = vfwprintf(fnull, fmt, c);
        va_end(c);
        fclose(fnull);
    }

    UnicodeString strOut;

    if (len > 0)
    {
        strOut.resize(len+1);
        va_list c;
        va_copy(c, s);
        len = vswprintf(&strOut[0], strOut.size(), fmt, c);
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

void Gwen::Utility::Strings::Split(const Gwen::UnicodeString& str,
                                   const Gwen::UnicodeString& seperator,
                                   Strings::UnicodeList& outbits, bool bLeave)
{
    size_t iOffset = 0;
    size_t iLength = str.length();
    size_t iSepLen = seperator.length();
    size_t i = str.find(seperator, 0);

    while (i != std::wstring::npos)
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

    return (float)atof(str.c_str());
}

float Gwen::Utility::Strings::To::Float(const Gwen::UnicodeString& str)
{
    return wcstod(str.c_str(), NULL);
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

bool Gwen::Utility::Strings::Wildcard(const TextObject& strWildcard, const TextObject& strHaystack)
{
    const UnicodeString& W = strWildcard.GetUnicode();
    const UnicodeString& H = strHaystack.GetUnicode();

    if (strWildcard == "*")
        return true;

    size_t iPos = W.find(L"*", 0);

    if (iPos == UnicodeString::npos)
        return strWildcard == strHaystack;

    // First half matches
    if (iPos > 0 && W.substr(0, iPos) != H.substr(0, iPos))
        return false;

    // Second half matches
    if (iPos != W.length()-1)
    {
        UnicodeString strEnd = W.substr(iPos+1, W.length());

        if (strEnd != H.substr(H.length()-strEnd.length(), H.length()))
            return false;
    }

    return true;
}

void Gwen::Utility::Strings::ToUpper(Gwen::UnicodeString& str)
{
    transform(str.begin(), str.end(), str.begin(), towupper);
}

void Gwen::Utility::Strings::Strip(Gwen::UnicodeString& str, const Gwen::UnicodeString& chars)
{
    Gwen::UnicodeString Source = str;
    str = L"";

    for (unsigned int i = 0; i < Source.length(); i++)
    {
        if (chars.find(Source[i]) != Gwen::UnicodeString::npos)
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



