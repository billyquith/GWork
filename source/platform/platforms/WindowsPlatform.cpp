/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
*  See license in Gwork.h
 */

#ifdef GWK_PLATFORM_WINDOWS

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x06000000
#else
#if _WIN32_WINNT < 0x06000000
#error Unsupported platform
#endif
#endif

#include <Gwork/Macros.h>
#include <Gwork/Utility.h>
#include <Gwork/Platform.h>
#include <Gwork/Input/Windows.h>

#include <windows.h>
#include <ShlObj.h>
#include <Shobjidl.h>

using namespace Gwk;
using namespace Gwk::Platform;

static const size_t FILESTRING_SIZE = 256;
static const size_t FILTERBUFFER_SIZE = 512;

static Gwk::Input::Windows GworkInput;

static LPCTSTR iCursorConversion[] =
{
    IDC_ARROW,
    IDC_IBEAM,
    IDC_SIZENS,
    IDC_SIZEWE,
    IDC_SIZENWSE,
    IDC_SIZENESW,
    IDC_SIZEALL,
    IDC_NO,
    IDC_WAIT,
    IDC_HAND
};

void Gwk::Platform::SetCursor(unsigned char iCursor)
{
    // Todo.. Properly.
    ::SetCursor(LoadCursor(nullptr, iCursorConversion[iCursor]));
}

void Gwk::Platform::GetCursorPos(Gwk::Point& po)
{
    POINT p;
    ::GetCursorPos(&p);
    po.x = p.x;
    po.y = p.y;
}

void Gwk::Platform::GetDesktopSize(int& w, int& h)
{
    w = GetSystemMetrics(SM_CXFULLSCREEN);
    h = GetSystemMetrics(SM_CYFULLSCREEN);
}

Gwk::String Gwk::Platform::GetClipboardText()
{
    if (!OpenClipboard(nullptr))
        return "";

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);

    if (hData == nullptr)
    {
        CloseClipboard();
        return "";
    }

    std::wstring buffer( static_cast<wchar_t*>(GlobalLock(hData)) );
    const String str = Utility::Narrow(buffer);
    GlobalUnlock(hData);
    CloseClipboard();

    return str;
}

bool Gwk::Platform::SetClipboardText(const Gwk::String& str)
{
    if (!OpenClipboard(nullptr))
        return false;

    EmptyClipboard();


    // Create a buffer to hold the string
    const std::wstring wstr( Utility::Widen(str) );
    const size_t dataSize = (wstr.length()+1)*sizeof(wchar_t);    
    HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, dataSize);

    // Copy the string into the buffer
    wchar_t* buffer = static_cast<wchar_t*>(GlobalLock(clipbuffer));
    wcscpy_s(buffer, dataSize, wstr.c_str());
    GlobalUnlock(clipbuffer);

    // Place it on the clipboard
    SetClipboardData(CF_UNICODETEXT, clipbuffer);
    CloseClipboard();

    return true;
}

static double GetPerformanceFrequency()
{
    static double Frequency = 0.0f;

    if (Frequency == 0.0f)
    {
        __int64 perfFreq;
        QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
        Frequency = 1.0 / perfFreq;
    }

    return Frequency;
}

float Gwk::Platform::GetTimeInSeconds()
{
    static float fCurrentTime = 0.0f;
    static __int64 iLastTime = 0;
    __int64 thistime;

    QueryPerformanceCounter((LARGE_INTEGER*)&thistime);
    double fSecondsDifference = (thistime-iLastTime)*GetPerformanceFrequency();

    if (fSecondsDifference > 0.1)
        fSecondsDifference = 0.1;

    fCurrentTime += fSecondsDifference;
    iLastTime = thistime;
    return fCurrentTime;
}

bool Gwk::Platform::FileOpen(const String& Name, const String& StartPath, const String& Extension,
                             String& filePathOut)
{
    char Filestring[FILESTRING_SIZE];
    String returnstring;
    char FilterBuffer[FILTERBUFFER_SIZE];
    {
        memset(FilterBuffer, 0, sizeof(FilterBuffer));
        memcpy(FilterBuffer, Extension.c_str(),
               Gwk::Min(Extension.length(), sizeof(FilterBuffer)));

        for (int i = 0; i < FILTERBUFFER_SIZE; i++)
        {
            if (FilterBuffer[i] == '|')
                FilterBuffer[i] = 0;
        }
    }
    OPENFILENAMEA opf;
    opf.hwndOwner = 0;
    opf.lpstrFilter = FilterBuffer;
    opf.lpstrCustomFilter = 0;
    opf.nMaxCustFilter = 0L;
    opf.nFilterIndex = 1L;
    opf.lpstrFile = Filestring;
    opf.lpstrFile[0] = '\0';
    opf.nMaxFile = FILESTRING_SIZE;
    opf.lpstrFileTitle = 0;
    opf.nMaxFileTitle = 50;
    opf.lpstrInitialDir = StartPath.c_str();
    opf.lpstrTitle = Name.c_str();
    opf.nFileOffset = 0;
    opf.nFileExtension = 0;
    opf.lpstrDefExt = "*.*";
    opf.lpfnHook = nullptr;
    opf.lCustData = 0;
    opf.Flags = (OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR)&~OFN_ALLOWMULTISELECT;
    opf.lStructSize = sizeof(OPENFILENAME);

    if (GetOpenFileNameA(&opf))
    {
        if (handler && fnCallback)
        {
            filePathOut = opf.lpstrFile;
        }
    }

    return true;
}

bool Gwk::Platform::FolderOpen(const String& Name, const String& StartPath,
                               String& filePathOut)
{
    IFileDialog* pfd = nullptr;
    bool bSuccess = false;

    if (CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER,
                         IID_PPV_ARGS(&pfd)) != S_OK)
        return bSuccess;

    DWORD dwOptions;

    if (pfd->GetOptions(&dwOptions) != S_OK)
    {
        pfd->Release();
        return bSuccess;
    }

    pfd->SetOptions(dwOptions|FOS_PICKFOLDERS);

    //
    // TODO: SetDefaultFolder -> StartPath
    //

    if (pfd->Show(nullptr) == S_OK)
    {
        IShellItem* psi;

        if (pfd->GetResult(&psi) == S_OK)
        {
            WCHAR* strOut = nullptr;

            if (psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &strOut) != S_OK)
                return bSuccess;

            if (handler && fnCallback)
            {
                filePathOut = Utility::Narrow(strOut); // set result
            }

            CoTaskMemFree(strOut);
            psi->Release();
            bSuccess = true;
        }
    }

    pfd->Release();
    return bSuccess;
}

bool Gwk::Platform::FileSave(const String& Name, const String& StartPath, const String& Extension,
                             String& filePathOut);
{
    char Filestring[FILESTRING_SIZE];
    String returnstring;
    char FilterBuffer[FILTERBUFFER_SIZE];
    {
        memset(FilterBuffer, 0, sizeof(FilterBuffer));
        memcpy(FilterBuffer, Extension.c_str(),
               Gwk::Min(Extension.size(), sizeof(FilterBuffer)));

        for (int i = 0; i < FILTERBUFFER_SIZE; i++)
        {
            if (FilterBuffer[i] == '|')
                FilterBuffer[i] = 0;
        }
    }
    OPENFILENAMEA opf;
    opf.hwndOwner = 0;
    opf.lpstrFilter = FilterBuffer;
    opf.lpstrCustomFilter = 0;
    opf.nMaxCustFilter = 0L;
    opf.nFilterIndex = 1L;
    opf.lpstrFile = Filestring;
    opf.lpstrFile[0] = '\0';
    opf.nMaxFile = FILESTRING_SIZE;
    opf.lpstrFileTitle = 0;
    opf.nMaxFileTitle = 50;
    opf.lpstrInitialDir = StartPath.c_str();
    opf.lpstrTitle = Name.c_str();
    opf.nFileOffset = 0;
    opf.nFileExtension = 0;
    opf.lpstrDefExt = "*.*";
    opf.lpfnHook = nullptr;
    opf.lCustData = 0;
    opf.Flags = (OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR) & ~OFN_ALLOWMULTISELECT;
    opf.lStructSize = sizeof(OPENFILENAME);

    if (GetSaveFileNameA(&opf))
    {
        if (handler && fnCallback)
        {
            filePathOut = opf.lpstrFile;
        }
    }

    return true;
}

void* Gwk::Platform::CreatePlatformWindow(int x, int y, int w, int h,
                                           const Gwk::String& strWindowTitle)
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    WNDCLASSA wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.style            = CS_OWNDC|CS_DROPSHADOW;
    wc.lpfnWndProc      = DefWindowProc;
    wc.hInstance        = GetModuleHandle(nullptr);
    wc.lpszClassName    = "GWK_Window_Class";
    wc.hCursor          = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassA(&wc);
    HWND hWindow = CreateWindowExA(WS_EX_APPWINDOW|WS_EX_ACCEPTFILES, wc.lpszClassName,
                                   strWindowTitle.c_str(), WS_POPUP|WS_VISIBLE, x, y, w, h, nullptr, nullptr,
                                   GetModuleHandle(nullptr), nullptr);
    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);
    // Curve the corners
    {
        HRGN rgn = CreateRoundRectRgn(0, 0, w+1, h+1, 4, 4);
        SetWindowRgn(hWindow, rgn, false);
    }
    return (void*)hWindow;
}

void Gwk::Platform::DestroyPlatformWindow(void* ptr)
{
    DestroyWindow((HWND)ptr);
    CoUninitialize();
}

bool Gwk::Platform::MessagePump(void* window)
{
    GworkInput.Initialize(ptarget);
    MSG msg;

    while (PeekMessage(&msg, (HWND)window, 0, 0, PM_REMOVE))
    {
        if (GworkInput.ProcessMessage(msg))
            continue;

        if (msg.message == WM_PAINT)
            ptarget->Redraw();

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // If the active window has changed then force a redraw of our canvas
    // since we might paint ourselves a different colour if we're inactive etc
    {
        static HWND g_lastFocus = nullptr;

        if (GetActiveWindow() != g_lastFocus)
        {
            g_lastFocus = GetActiveWindow();
            return true;
        }
    }
    
    return false;
}

void Gwk::Platform::SetBoundsPlatformWindow(void* ptr, int x, int y, int w, int h)
{
    SetWindowPos((HWND)ptr, HWND_NOTOPMOST, x, y, w, h,
                 SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOSENDCHANGING);
    // Curve the corners
    {
        HRGN rgn = CreateRoundRectRgn(0, 0, w+1, h+1, 4, 4);
        SetWindowRgn((HWND)ptr, rgn, false);
    }
}

void Gwk::Platform::SetWindowMaximized(void* ptr, bool bMax, Gwk::Point& newPos,
                                        Gwk::Point& newSize)
{
    if (bMax)
    {
        ShowWindow((HWND)ptr, SW_SHOWMAXIMIZED);
        RECT rect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);    // size excluding
                                                               // task bar
        SetWindowPos((HWND)ptr, HWND_NOTOPMOST, rect.left, rect.top, rect.right-rect.left,
                     rect.bottom-rect.top,
                     SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS|SWP_NOSENDCHANGING);
        // Remove the corner curves
        {
            SetWindowRgn((HWND)ptr, nullptr, false);
        }
    }
    else
    {
        ShowWindow((HWND)ptr, SW_RESTORE);
        // Curve the corners
        {
            RECT r;
            GetWindowRect((HWND)ptr, &r);
            HRGN rgn = CreateRoundRectRgn(0, 0, (r.right-r.left)+1, (r.bottom-r.top)+1, 4, 4);
            SetWindowRgn((HWND)ptr, rgn, false);
        }
    }

    RECT r;
    GetWindowRect((HWND)ptr, &r);
    newSize.x = r.right-r.left;
    newSize.y = r.bottom-r.top;
    newPos.x = r.left;
    newPos.y = r.top;
}

void Gwk::Platform::SetWindowMinimized(void* ptr, bool bMinimized)
{
    if (bMinimized)
        ShowWindow((HWND)ptr, SW_SHOWMINIMIZED);
    else
        ShowWindow((HWND)ptr, SW_RESTORE);
}

bool Gwk::Platform::IsFocussedPlatformWindow(void* ptr)
{
    return GetActiveWindow() == (HWND)ptr;
}

void Gwk::Platform::Sleep(unsigned int iMS)
{
    ::Sleep(iMS);
}

#endif // GWK_PLATFORM_WINDOWS
