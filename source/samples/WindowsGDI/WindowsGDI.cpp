/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#define GWK_SAMPLE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <objbase.h>
#include <GdiPlus.h>

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Windows.h>
#include <Gwork/Renderers/GDIPlus.h>

#pragma comment( lib, "gdiplus.lib" )


HWND CreateGameWindow(void)
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.style            = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    wc.lpfnWndProc      = DefWindowProc;
    wc.hInstance        = GetModuleHandle(nullptr);
    wc.lpszClassName    = L"GworkWindow";
    wc.hCursor          = LoadCursor(nullptr, IDC_ARROW);
    RegisterClass(&wc);
    HWND hWindow = CreateWindowEx((WS_EX_APPWINDOW|WS_EX_WINDOWEDGE), wc.lpszClassName,
                                  L"Gwork - GDI+ Sample",
                                  (WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|
                                   WS_CLIPCHILDREN)&~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME),
                                  -1, -1, 1004, 650, nullptr, nullptr, GetModuleHandle(
                                      nullptr), nullptr);
    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);
    return hWindow;
}

HWND g_hWND = nullptr;

int main()
{
    //
    // Create a new window
    //
    g_hWND = CreateGameWindow();
    //
    // Create a Gwork GDI+ Renderer
    // Note: we're using the buffered version.
    // This version draws to a texture and then draws that texture to the window
    // This prevents all the crazy flickering (test with Gwk::Renderer::GDIPlus
    // to see)
    //
    Gwk::Renderer::GDIPlusBuffered* renderer = new Gwk::Renderer::GDIPlusBuffered(g_hWND);
    //
    // Create a Gwork skin
    //
    // Gwk::Skin::Simple skin;
    Gwk::Skin::TexturedBase* skin = new Gwk::Skin::TexturedBase(renderer);
    skin->Init("DefaultSkin.png");
    //
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    //
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(skin);
    canvas->SetSize(998, 650-24);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));
    //
    // Create our unittest control (which is a Window with controls in it)
    //
    UnitTest* unit = new UnitTest(canvas);
    unit->SetPos(10, 10);
    //
    // Create a Windows Control helper
    // (Processes Windows MSG's and fires input at Gwork)
    //
    Gwk::Input::Windows GworkInput;
    GworkInput.Initialize(canvas);
    //
    // Begin the main game loop
    //
    MSG msg;

    while (true)
    {
        // Skip out if the window is closed
        if (!IsWindowVisible(g_hWND))
            break;

        // If we have a message from windows..
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            // .. give it to the input handler to process
            GworkInput.ProcessMessage(msg);

            // if it's QUIT then quit..
            if (msg.message == WM_QUIT)
                break;

            if (msg.message == WM_PAINT)
            {
                // This doesn't actually draw it, it just marks it
                // so it will redraw when next checked (NeedsRedraw)
                canvas->Redraw();
            }

            // Handle the regular window stuff..
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // If Gwork's Canvas needs a redraw then redraw it
        //
        // In your game you would probably just draw it
        //  every frame. But we have the option of only
        //  drawing it when it needs it too.
        //
        if (canvas->NeedsRedraw())
            canvas->RenderCanvas();
    }

    delete canvas;
    delete skin;
    delete renderer;
}
