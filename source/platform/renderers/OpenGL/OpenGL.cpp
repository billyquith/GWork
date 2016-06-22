
#include <Gwork/Renderers/OpenGL.h>
#include <Gwork/Utility.h>
#include <Gwork/Font.h>
#include <Gwork/Texture.h>
#include <Gwork/WindowProvider.h>

#include <math.h>

#include "GL/glew.h"
#include "FreeImage/FreeImage.h"


namespace Gwk
{
namespace Renderer
{
OpenGL::OpenGL()
{
    m_vertNum = 0;
    m_context = nullptr;
    ::FreeImage_Initialise();

    for (int i = 0; i < MaxVerts; i++)
    {
        m_vertices[ i ].z = 0.5f;
    }
}

OpenGL::~OpenGL()
{
    ::FreeImage_DeInitialise();
}

void OpenGL::Init()
{
}

void OpenGL::Begin()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, 1.0f);
    glEnable(GL_BLEND);
}

void OpenGL::End()
{
    Flush();
}

void OpenGL::Flush()
{
    if (m_vertNum == 0)
        return;

    glVertexPointer(3, GL_FLOAT,  sizeof(Vertex), (void*)&m_vertices[0].x);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)&m_vertices[0].r);
    glEnableClientState(GL_COLOR_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)&m_vertices[0].u);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertNum);
    m_vertNum = 0;
    glFlush();
}

void OpenGL::AddVert(int x, int y, float u, float v)
{
    if (m_vertNum >= MaxVerts-1)
        Flush();

    m_vertices[ m_vertNum ].x = (float)x;
    m_vertices[ m_vertNum ].y = (float)y;
    m_vertices[ m_vertNum ].u = u;
    m_vertices[ m_vertNum ].v = v;
    m_vertices[ m_vertNum ].r = m_color.r;
    m_vertices[ m_vertNum ].g = m_color.g;
    m_vertices[ m_vertNum ].b = m_color.b;
    m_vertices[ m_vertNum ].a = m_color.a;
    m_vertNum++;
}

void OpenGL::DrawFilledRect(Gwk::Rect rect)
{
    GLboolean texturesOn;
    glGetBooleanv(GL_TEXTURE_2D, &texturesOn);

    if (texturesOn)
    {
        Flush();
        glDisable(GL_TEXTURE_2D);
    }

    Translate(rect);
    AddVert(rect.x, rect.y);
    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x, rect.y+rect.h);
    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x+rect.w, rect.y+rect.h);
    AddVert(rect.x, rect.y+rect.h);
}

void OpenGL::SetDrawColor(Gwk::Color color)
{
    glColor4ubv((GLubyte*)&color);
    m_color = color;
}

void OpenGL::StartClip()
{
    Flush();
    Gwk::Rect rect = ClipRegion();
    // OpenGL's coords are from the bottom left
    // so we need to translate them here.
    {
        GLint view[4];
        glGetIntegerv(GL_VIEWPORT, &view[0]);
        rect.y = view[3]-(rect.y+rect.h);
    }
    glScissor(rect.x*Scale(), rect.y*Scale(), rect.w*Scale(), rect.h*Scale());
    glEnable(GL_SCISSOR_TEST);
}

void OpenGL::EndClip()
{
    Flush();
    glDisable(GL_SCISSOR_TEST);
}

void OpenGL::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect, float u1, float v1,
                              float u2, float v2)
{
    GLuint* tex = (GLuint*)texture->data;

    // Missing image, not loaded properly?
    if (!tex)
        return DrawMissingImage(rect);

    Translate(rect);
    GLuint boundtex;
    GLboolean texturesOn;
    glGetBooleanv(GL_TEXTURE_2D, &texturesOn);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&boundtex);

    if (!texturesOn || *tex != boundtex)
    {
        Flush();
        glBindTexture(GL_TEXTURE_2D, *tex);
        glEnable(GL_TEXTURE_2D);
    }

    AddVert(rect.x, rect.y,            u1, v1);
    AddVert(rect.x+rect.w, rect.y,       u2, v1);
    AddVert(rect.x, rect.y+rect.h,   u1, v2);
    AddVert(rect.x+rect.w, rect.y,       u2, v1);
    AddVert(rect.x+rect.w, rect.y+rect.h, u2, v2);
    AddVert(rect.x, rect.y+rect.h, u1, v2);
}

void OpenGL::LoadTexture(Gwk::Texture* texture)
{
    const std::string &fileName = texture->name;
    FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileType(fileName.c_str());

    if (imageFormat == FIF_UNKNOWN)
        imageFormat = FreeImage_GetFIFFromFilename(fileName.c_str());

    // Image failed to load..
    if (imageFormat == FIF_UNKNOWN)
    {
        texture->failed = true;
        return;
    }

    // Try to load the image..
    FIBITMAP* bits = FreeImage_Load(imageFormat, fileName.c_str());

    if (!bits)
    {
        texture->failed = true;
        return;
    }

    // Convert to 32bit
    FIBITMAP* bits32 = FreeImage_ConvertTo32Bits(bits);
    FreeImage_Unload(bits);

    if (!bits32)
    {
        texture->failed = true;
        return;
    }

    // Flip
    ::FreeImage_FlipVertical(bits32);

    // Create a little texture pointer..
    GLuint* pglTexture = new GLuint;

    // Sort out our Gwork texture
    texture->data = pglTexture;
    texture->width = FreeImage_GetWidth(bits32);
    texture->height = FreeImage_GetHeight(bits32);

    // Create the opengl texture
    glGenTextures(1, pglTexture);
    glBindTexture(GL_TEXTURE_2D, *pglTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef FREEIMAGE_BIGENDIAN
    GLenum format = GL_RGBA;
#else
    GLenum format = GL_BGRA;
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, format,
                 GL_UNSIGNED_BYTE, (const GLvoid*)FreeImage_GetBits(bits32));
    FreeImage_Unload(bits32);
}

void OpenGL::FreeTexture(Gwk::Texture* texture)
{
    GLuint* tex = (GLuint*)texture->data;

    if (!tex)
        return;

    glDeleteTextures(1, tex);
    delete tex;
    texture->data = nullptr;
}

Gwk::Color OpenGL::PixelColour(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                const Gwk::Color& col_default)
{
    GLuint* tex = (GLuint*)texture->data;

    if (!tex)
        return col_default;

    unsigned int iPixelSize = sizeof(unsigned char)*4;
    glBindTexture(GL_TEXTURE_2D, *tex);
    unsigned char* data =
        (unsigned char*)malloc(iPixelSize*texture->width*texture->height);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    unsigned int iOffset = (y*texture->width+x)*4;
    Gwk::Color c;
    c.r = data[0+iOffset];
    c.g = data[1+iOffset];
    c.b = data[2+iOffset];
    c.a = data[3+iOffset];
    //
    // Retrieving the entire texture for a single pixel read
    // is kind of a waste - maybe cache this pointer in the texture
    // data and then release later on? It's never called during runtime
    // - only during initialization.
    //
    free(data);
    return c;
}

bool OpenGL::InitializeContext(Gwk::WindowProvider* window)
{
#ifdef _WIN32
    HWND hwnd = (HWND)window->GetWindow();

    if (!hwnd)
        return false;

    HDC hDC = GetDC(hwnd);
    //
    // Set the pixel format
    //
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int iFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, iFormat, &pfd);
    HGLRC hRC;
    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    RECT r;

    if (GetClientRect(hwnd, &r))
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(r.left, r.right, r.bottom, r.top, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, r.right-r.left, r.bottom-r.top);
    }

    m_context = (void*)hRC;
    return true;
#endif // ifdef _WIN32
    return false;
}

bool OpenGL::ShutdownContext(Gwk::WindowProvider* window)
{
#ifdef _WIN32
    wglDeleteContext((HGLRC)m_context);
    return true;
#endif
    return false;
}

bool OpenGL::PresentContext(Gwk::WindowProvider* window)
{
#ifdef _WIN32
    HWND hwnd = (HWND)window->GetWindow();

    if (!hwnd)
        return false;

    HDC hDC = GetDC(hwnd);
    SwapBuffers(hDC);
    return true;
#endif
    return false;
}

bool OpenGL::ResizedContext(Gwk::WindowProvider* window, int w, int h)
{
#ifdef _WIN32
    RECT r;

    if (GetClientRect((HWND)window->GetWindow(), &r))
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(r.left, r.right, r.bottom, r.top, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glViewport(0, 0, r.right-r.left, r.bottom-r.top);
    }

    return true;
#endif // ifdef _WIN32
    return false;
}

bool OpenGL::BeginContext(Gwk::WindowProvider* window)
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    return true;
}

bool OpenGL::EndContext(Gwk::WindowProvider* window)
{
    return true;
}

} // namespace Renderer
} // namespace Gwk
