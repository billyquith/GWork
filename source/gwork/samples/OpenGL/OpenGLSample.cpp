
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Gwork/Gwork.h"
#include "Gwork/Skins/Simple.h"
#include "Gwork/Skins/TexturedBase.h"
#include "Gwork/UnitTest/UnitTest.h"
#include "Gwork/Input/Windows.h"

#ifdef USE_DEBUG_FONT
#include "Gwork/Renderers/OpenGL_DebugFont.h"
#else
#include "Gwork/Renderers/OpenGL.h"
#endif
#include "gl/glew.h"

HWND CreateGameWindow(void)
{
    WNDCLASSW wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.style            = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
    wc.lpfnWndProc      = DefWindowProc;
    wc.hInstance        = GetModuleHandle(NULL);
    wc.lpszClassName    = L"GworkWindow";
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    RegisterClassW(&wc);
#ifdef USE_DEBUG_FONT
    HWND hWindow = CreateWindowExW((WS_EX_APPWINDOW|WS_EX_WINDOWEDGE), wc.lpszClassName,
                                   L"Gwork - OpenGL Sample (Using embedded debug font renderer)",
                                   (WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|
                                    WS_CLIPCHILDREN)&~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME),
                                   -1, -1, 1004, 650, NULL, NULL, GetModuleHandle(
                                       NULL), NULL);
#else
    HWND hWindow = CreateWindowExW((WS_EX_APPWINDOW|WS_EX_WINDOWEDGE), wc.lpszClassName,
                                   L"Gwork - OpenGL Sample (No cross platform way to render fonts in OpenGL)",
                                   (WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|
                                    WS_CLIPCHILDREN)&~(WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_THICKFRAME), -1, -1, 1004, 650, NULL, NULL,
                                   GetModuleHandle(NULL), NULL);
#endif // ifdef USE_DEBUG_FONT
    ShowWindow(hWindow, SW_SHOW);
    SetForegroundWindow(hWindow);
    SetFocus(hWindow);
    return hWindow;
}

HWND g_pHWND = NULL;

HGLRC CreateOpenGLDeviceContext()
{
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);      // just its size
    pfd.nVersion = 1;   // always 1
    pfd.dwFlags = PFD_SUPPORT_OPENGL|   // OpenGL support - not DirectDraw
                  PFD_DOUBLEBUFFER|     // double buffering support
                  PFD_DRAW_TO_WINDOW;   // draw to the app window, not to a
                                        // bitmap image
    pfd.iPixelType = PFD_TYPE_RGBA;     // red, green, blue, alpha for each
                                        // pixel
    pfd.cColorBits = 24;                // 24 bit == 8 bits for red, 8 for
                                        // green, 8 for blue.
    // This count of color bits EXCLUDES alpha.
    pfd.cDepthBits = 32;                // 32 bits to measure pixel depth.
    int pixelFormat = ChoosePixelFormat(GetDC(g_pHWND), &pfd);

    if (pixelFormat == 0)
        FatalAppExit(NULL, TEXT("ChoosePixelFormat() failed!"));

    SetPixelFormat(GetDC(g_pHWND), pixelFormat, &pfd);
    HGLRC OpenGLContext = wglCreateContext(GetDC(g_pHWND));
    wglMakeCurrent(GetDC(g_pHWND), OpenGLContext);
    RECT r;

    if (GetClientRect(g_pHWND, &r))
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(r.left, r.right, r.bottom, r.top, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, r.right-r.left, r.bottom-r.top);
    }

    return OpenGLContext;
}

int main()
{
    //
    // Create a new window
    //
    g_pHWND = CreateGameWindow();
    //
    // Create OpenGL Device
    //
    HGLRC OpenGLContext = CreateOpenGLDeviceContext();
    //
    // Create a Gwork OpenGL Renderer
    //
#ifdef USE_DEBUG_FONT
    Gwk::Renderer::OpenGL* pRenderer = new Gwk::Renderer::OpenGL_DebugFont();
#else
    Gwk::Renderer::OpenGL* pRenderer = new Gwk::Renderer::OpenGL();
#endif
    pRenderer->Init();
    //
    // Create a Gwork skin
    //
    Gwk::Skin::TexturedBase* pSkin = new Gwk::Skin::TexturedBase(pRenderer);
    pSkin->Init("DefaultSkin.png");
    //
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    //
    Gwk::Controls::Canvas* pCanvas = new Gwk::Controls::Canvas(pSkin);
    pCanvas->SetSize(998, 650-24);
    pCanvas->SetDrawBackground(true);
    pCanvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));
    //
    // Create our unittest control (which is a Window with controls in it)
    //
    UnitTest* pUnit = new UnitTest(pCanvas);
    pUnit->SetPos(10, 10);
    //
    // Create a Windows Control helper
    // (Processes Windows MSG's and fires input at Gwork)
    //
    Gwk::Input::Windows GworkInput;
    GworkInput.Initialize(pCanvas);
    //
    // Begin the main game loop
    //
    MSG msg;

    while (true)
    {
        // Skip out if the window is closed
        if (!IsWindowVisible(g_pHWND))
            break;

        // If we have a message from windows..
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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

        // Main OpenGL Render Loop
        {
            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            pCanvas->RenderCanvas();
            SwapBuffers(GetDC(g_pHWND));
        }
    }

    // Clean up OpenGL
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(OpenGLContext);
    delete pCanvas;
    delete pSkin;
    delete pRenderer;
}
