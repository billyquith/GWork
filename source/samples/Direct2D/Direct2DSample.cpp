/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#define GWK_SAMPLE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Windows.h>
#include <Gwork/Renderers/Direct2D.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "windowscodecs.lib")

HWND g_hWND = nullptr;
ID2D1Factory*           g_d2DFactory = nullptr;
IDWriteFactory*         g_dWriteFactory = nullptr;
IWICImagingFactory*     g_wICFactory = nullptr;
ID2D1HwndRenderTarget*  g_rT = nullptr; // this is device-specific

Gwk::Renderer::Direct2D* g_renderer = nullptr;

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
                                  L"Gwork - Direct 2D Sample",
                                  (WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|
                                   WS_CLIPCHILDREN)&~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME),
                                  -1, -1, 1004, 650, nullptr, nullptr, GetModuleHandle(
                                      nullptr), nullptr);
    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);
    return hWindow;
}

HRESULT createDeviceResources();
void    discardDeviceResources();
void    runSample();

//
// Program starts here
//
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    HRESULT hr = D2D1CreateFactory(
        D2D1_FACTORY_TYPE_SINGLE_THREADED,
        &g_d2DFactory
        );
    hr = DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        reinterpret_cast<IUnknown**>(&g_dWriteFactory)
        );
    hr = CoInitialize(nullptr);
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        reinterpret_cast<void**>(&g_wICFactory)
        );
    g_hWND = CreateGameWindow();
    createDeviceResources();
    //
    // Create a Gwork Direct2D renderer
    //
    g_renderer = new Gwk::Renderer::Direct2D(g_rT, g_dWriteFactory, g_wICFactory);
    runSample();
    delete g_renderer;
    g_renderer = nullptr;

    if (g_rT != nullptr)
    {
        g_rT->Release();
        g_rT = nullptr;
    }
}

HRESULT createDeviceResources()
{
    HRESULT hr = S_OK;

    if (!g_rT)
    {
        RECT rc;
        GetClientRect(g_hWND, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right-rc.left,
            rc.bottom-rc.top
            );
        // Create a Direct2D render target.
        hr = g_d2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(g_hWND, size),
            &g_rT
            );

        if (SUCCEEDED(hr) && g_renderer != nullptr)
        {
            g_rT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
            g_renderer->DeviceAcquired(g_rT);
        }
    }

    return hr;
}

void discardDeviceResources()
{
    if (g_rT != nullptr)
    {
        g_rT->Release();
        g_rT = nullptr;
    }
}

void runSample()
{
    RECT FrameBounds;
    GetClientRect(g_hWND, &FrameBounds);
    //
    // Create a Gwork skin
    //
    Gwk::Skin::TexturedBase skin(g_renderer);
    skin.Init("DefaultSkin.png");
    //
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    //
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(&skin);
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

        {
            if (SUCCEEDED(createDeviceResources()))
            {
                g_rT->BeginDraw();
                g_rT->SetTransform(D2D1::Matrix3x2F::Identity());
                g_rT->Clear(D2D1::ColorF(D2D1::ColorF::White));
                // This is how easy it is to render Gwork!
                canvas->RenderCanvas();
                HRESULT hr = g_rT->EndDraw();

                if (hr == D2DERR_RECREATE_TARGET)
                {
                    discardDeviceResources();
                    g_renderer->DeviceLost();
                }
            }
        }
    }

    delete canvas;
}
