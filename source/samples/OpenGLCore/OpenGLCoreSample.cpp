/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <GL/glew.h>
#ifdef USE_DEBUG_FONT
#   include <Gwork/Renderers/OpenGL_DebugFont.h>
#else
#   include <Gwork/Renderers/OpenGLCore.h>
#endif
#include <Gwork/Gwork.h>
#include <Gwork/Platform.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/OpenGL.h>
#include <iostream>

static Gwk::Input::GLFW GworkInput;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    const Gwk::Input::GLFW::KeyEvent evt = { key, scancode, action, mods };
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

void glDebugOutput(GLenum source,
                   GLenum type,
                   GLuint id,
                   GLenum severity,
                   GLsizei,
                   const GLchar *message,
                   const void *)
{
    // ignore non-significant error/warning codes
//    if(id == 131169 ||
//       id == 131185 ||
//       id == 131218 ||
//       id == 131204 ||
//       id == 8 ||
//       id == 22 || /* CPU mapping a bisy "streamed data" BO stalled */
//       id == 20 || /* GTT mapping a busy "miptree" BO stalled */
//       id == 14 || /* CPU mapping a busy "miptree" BO stalled */
//       id == 18    /* CPU mapping a busy "streamed data" BO stalled */)
//    {
//        return;
//    }

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    std::stringstream ss;
    ss << "Debug message (" << id << "): " <<  message << std::endl;

    ss << "Source: ";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             ss << "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     ss << "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           ss << "Other"; break;
    default:                              ss << "Unexpected"; break;;
    } ss << std::endl;

    ss << "Type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               ss << "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         ss << "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         ss << "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              ss << "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           ss << "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               ss << "Other"; break;
    default:                                ss << "Unexpected"; break;
    } ss << std::endl;

    ss << "Severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         ss << "high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       ss << "medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          ss << "low"; break;
    default:                             ss << "unexpected"; break;
    } ss << std::endl;
    ss << std::endl;

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        std::cout << "Error: " << ss.str();
    }
    else
    {
        std::cout << "Warning: " << ss.str();
    }
}

int main()
{
    const Gwk::Point screenSize(1024,768);

    if (!glfwInit())
        return -1;


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    // Create a new window
    GLFWwindow* window = glfwCreateWindow(screenSize.x, screenSize.y,
                                          "Gwork OpenGL Core Profile Sample", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    GLuint error;
    if ((error = glewInit()) != GLEW_OK)
    {
        std::cout << "Glew init error: " << glewGetErrorString(error) << std::endl;
        glfwTerminate();
        return -1;
    }

    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

        glDebugMessageCallback(&glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    Gwk::Platform::RelativeToExecutablePaths paths(GWORK_RESOURCE_DIR);
    Gwk::Renderer::OpenGLResourceLoader loader(paths);

    // Create a Gwork OpenGL Renderer
#ifdef USE_DEBUG_FONT
    Gwk::Renderer::OpenGL* renderer = new Gwk::Renderer::OpenGL_DebugFont();
#else
    Gwk::Renderer::OpenGLCore* renderer =
        new Gwk::Renderer::OpenGLCore(loader, Gwk::Rect(Gwk::Point(0,0), screenSize));
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
    auto unit = new TestFrame(canvas);
//    auto unit = new Gwk::Controls::Button(canvas);
//    unit->SetText("SampleText");
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
