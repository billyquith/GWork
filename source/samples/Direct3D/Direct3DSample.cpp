/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from
                                        // Windows headers
// Windows Header Files:
#include <windows.h>

#include <d3d9.h>
#include <D3dx9core.h>
#include <D3dx9math.h>
#include <DxErr.h>

// C RunTime Header Files
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Windows.h>
#include <Gwork/Renderers/DirectX9.h>

HWND g_hWND = nullptr;
LPDIRECT3D9 g_D3D = nullptr;
IDirect3DDevice9*       g_D3DDevice = nullptr;
D3DPRESENT_PARAMETERS g_D3DParams;

//
// Windows bullshit to create a Window to render to.
//
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
                                  L"Gwork - Direct 3D Sample",
                                  (WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|
                                   WS_CLIPCHILDREN)&~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME),
                                  -1, -1, 1004, 650, nullptr, nullptr, GetModuleHandle(
                                      nullptr), nullptr);
    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);
    return hWindow;
}

//
// Typical DirectX stuff to create a D3D device
//
void ResetD3DDevice()
{
    g_D3DDevice->Reset(&g_D3DParams);
}

void CreateD3DDevice()
{
    ZeroMemory(&g_D3DParams, sizeof(g_D3DParams));
    RECT ClientRect;
    GetClientRect(g_hWND, &ClientRect);
    g_D3DParams.Windowed = TRUE;
    g_D3DParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_D3DParams.BackBufferWidth = ClientRect.right;
    g_D3DParams.BackBufferHeight = ClientRect.bottom;
    g_D3DParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    g_D3DParams.BackBufferFormat = D3DFMT_X8R8G8B8;
    // g_D3DParams.EnableAutoDepthStencil = TRUE;
    // g_D3DParams.AutoDepthStencilFormat = D3DFMT_D24S8;
    g_D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    HRESULT hr = g_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_D3DParams,
                                      &g_D3DDevice);

    if (FAILED(hr))
    {
        OutputDebugString(L"Couldn't create D3D Device for some reason!");
        return;
    }
}

//
// Program starts here
//
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    //
    // Create a window and attach directx to it
    //
    g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
    g_hWND = CreateGameWindow();
    CreateD3DDevice();
    RECT FrameBounds;
    GetClientRect(g_hWND, &FrameBounds);
    //
    // Create a Gwork DirectX renderer
    //
    Gwk::Renderer::DirectX9* renderer = new Gwk::Renderer::DirectX9(g_D3DDevice);
    //
    // Create a Gwork skin
    //
    Gwk::Skin::TexturedBase* skin = new Gwk::Skin::TexturedBase(renderer);
    skin->Init("DefaultSkin.png");
    //
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    //
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(skin);
    canvas->SetSize(FrameBounds.right, FrameBounds.bottom);
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

            // Handle the regular window stuff..
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Normal DirectX rendering loop
            g_D3DDevice->BeginScene();
            g_D3DDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1, 0);
            // This is how easy it is to render Gwork!
            canvas->RenderCanvas();
            g_D3DDevice->EndScene();
            g_D3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
        }
    }

    delete canvas;
    delete skin;
    delete renderer;

    if (g_D3DDevice)
    {
        g_D3DDevice->Release();
        g_D3DDevice = nullptr;
    }

    if (g_D3D)
    {
        g_D3D->Release();
        g_D3D = nullptr;
    }
}
