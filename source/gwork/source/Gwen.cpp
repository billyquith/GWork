/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <stdio.h>
#include <stdarg.h>


namespace Gwk
{
    // Globals
    GWK_EXPORT Controls::Base* HoveredControl = NULL;
    GWK_EXPORT Controls::Base* KeyboardFocus = NULL;
    GWK_EXPORT Controls::Base* MouseFocus = NULL;

    namespace Debug
    {
        void Msg(const char* str, ...)
        {
            char strOut[1024];
            va_list s;

            va_start(s, str);
#ifdef WIN32
            vsnprintf_s(strOut, sizeof(strOut), str, s);
            va_end(s);
            OutputDebugStringA(strOut);
#else
            vsnprintf(strOut, sizeof(strOut), str, s);
            va_end(s);
            puts(strOut);
#endif
        }

#ifdef UNICODE
        void Msg(const wchar_t* str, ...)
        {
            wchar_t strOut[1024];
            va_list s;
            va_start(s, str);
            vswprintf(strOut, sizeof(strOut), str, s);
            va_end(s);
            GwkUtil_OutputDebugWideString(strOut);
        }

#endif // ifdef UNICODE

        void AssertCheck(bool b, const char* strMsg)
        {
            if (!b)
            {
                Msg("Assert: %s\n", strMsg);
#ifdef WIN32
                MessageBoxA(NULL, strMsg, "Assert", MB_ICONEXCLAMATION | MB_OK);
                _asm { int 3 }      // Break to debugger.
#endif
                
#ifdef __APPLE__
                __builtin_trap();   // Break to debugger.
#endif
            }
        }

    }
}
