/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Platform.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/GLFW3.h>
#ifdef USE_DEBUG_FONT
#   include <Gwork/Renderers/OpenGL_DebugFont.h>
#else
#   include <Gwork/Renderers/OpenGL.h>
#endif
#include <GLFW/glfw3.h>

static Gwk::Input::GLFW3 GworkInput;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const Gwk::Input::GLFW3::KeyEvent evt = { key, scancode, action, mods };
    GworkInput.ProcessKeyEvent(evt);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    GworkInput.ProcessMouseMove(xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    GworkInput.ProcessMouseButtons(button, action, mods);
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    GworkInput.ProcessScroll(xoffset, yoffset);
}


int main()
{
    const Gwk::Point screenSize(1024,768);

    if (!glfwInit())
        return -1;

    // Create a new window
    GLFWwindow* window = glfwCreateWindow(screenSize.x, screenSize.y,
                                          "Gwork OpenGL Sample", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    Gwk::Platform::RelativeToExecutablePaths paths(GWK_SAMPLE_RESOURCE_DIR);

    // Create a Gwork OpenGL Renderer
#ifdef USE_DEBUG_FONT
    Gwk::Renderer::OpenGL* renderer = new Gwk::Renderer::OpenGL_DebugFont();
#else
    Gwk::Renderer::OpenGL* renderer =
        new Gwk::Renderer::OpenGL(paths, Gwk::Rect(Gwk::Point(0,0), screenSize));
#endif
    renderer->Init();

    // Create a Gwork skin
    Gwk::Skin::TexturedBase* skin = new Gwk::Skin::TexturedBase(renderer);
    skin->Init("DefaultSkin.png");
    skin->SetDefaultFont("OpenSans.ttf", 11);

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* canvas = new Gwk::Controls::Canvas(skin);
    canvas->SetSize(screenSize.x, screenSize.y);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    auto unit = Gwk::Test::CreateTests(canvas);
    GworkInput.Initialize(canvas);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Begin the main game loop
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        canvas->RenderCanvas();
        glfwSwapBuffers(window);

        glfwPollEvents();

        Gwk::Platform::Sleep(0);
    }

    delete unit;
    delete canvas;
    delete skin;
    delete renderer;

    // Clean up OpenGL
    glfwTerminate();

    return EXIT_SUCCESS;
}
