/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/OpenGL.h>
#include <Gwork/PlatformTypes.h>

#include <GLFW/glfw3.h>
#include <math.h>
#include <sys/stat.h>
#include <stdio.h>

//#define STBI_ASSERT(x)  // comment in for no asserts
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "stb_truetype.h"

namespace Gwk
{
namespace Renderer
{
    
// See "Font Size in Pixels or Points" in "stb_truetype.h"
const float c_pixToPoints = 1.333f;

static const int c_texsz = 256;   // TODO - fix this hack.

OpenGL::OpenGL(const Rect& viewRect)
:   m_viewRect(viewRect)
,   m_vertNum(0)
,   m_context(nullptr)
{
    for (int i = 0; i < MaxVerts; i++)
    {
        m_vertices[ i ].z = 0.5f;
    }
}

OpenGL::~OpenGL()
{
}

void OpenGL::Init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(m_viewRect.x, m_viewRect.x + m_viewRect.w,
            m_viewRect.y, m_viewRect.y + m_viewRect.h,
            -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glViewport(m_viewRect.x, m_viewRect.y, m_viewRect.w, m_viewRect.h);
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

    // OpenGL origin is bottom-left. Gwork origin is top-left.
    m_vertices[ m_vertNum ].x = float(x);
    m_vertices[ m_vertNum ].y = float(m_viewRect.h - y);
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
    rect.y = m_viewRect.h - (rect.y + rect.h);

    glScissor(rect.x * Scale(), rect.y * Scale(),
              rect.w * Scale(), rect.h * Scale());
    glEnable(GL_SCISSOR_TEST);
}

void OpenGL::EndClip()
{
    Flush();
    glDisable(GL_SCISSOR_TEST);
}

void OpenGL::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                              float u1, float v1, float u2, float v2)
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
    
    AddVert(rect.x, rect.y,             u1, v1);
    AddVert(rect.x+rect.w, rect.y,      u2, v1);
    AddVert(rect.x, rect.y+rect.h,      u1, v2);
    AddVert(rect.x+rect.w, rect.y,      u2, v1);
    AddVert(rect.x+rect.w, rect.y+rect.h, u2, v2);
    AddVert(rect.x, rect.y+rect.h,      u1, v2);
}

void OpenGL::LoadTexture(Gwk::Texture* texture)
{
    const std::string &fileName = texture->name;
    
    int x,y,n;
    unsigned char *data = stbi_load(fileName.c_str(), &x, &y, &n, 4);
    
    // Image failed to load..
    if (!data)
    {
        texture->failed = true;
        return;
    }

    // Create a little texture pointer..
    GLuint* pglTexture = new GLuint;

    texture->data = pglTexture;
    texture->width = x;
    texture->height = y;

    // Create the opengl texture
    glGenTextures(1, pglTexture);
    glBindTexture(GL_TEXTURE_2D, *pglTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, format,
                 GL_UNSIGNED_BYTE, (const GLvoid*)data);
    
    stbi_image_free(data);
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
        (unsigned char*)malloc(iPixelSize * texture->width * texture->height);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    unsigned int iOffset = (y*texture->width+x)*4;
    
    Color c(data[0+iOffset], data[1+iOffset], data[2+iOffset], data[3+iOffset]);

    // Retrieving the entire texture for a single pixel read
    // is kind of a waste - maybe cache this pointer in the texture
    // data and then release later on? It's never called during runtime
    // - only during initialization.
    free(data);
    
    return c;
}

void OpenGL::LoadFont(Gwk::Font* font)
{
    std::string fontName(font->facename);
    
    if (fontName.find(".ttf") == std::string::npos)
        fontName += ".ttf";
    
    font->realsize = font->size * Scale() * c_pixToPoints;
    
    FILE* f = fopen(fontName.c_str(), "rb");
    if (!f)
    {
        font->data = nullptr;
        font->facename = "";
        return;
    }
    
    struct stat stat_buf;
    int rc = stat(fontName.c_str(), &stat_buf);
    size_t fsz = rc == 0 ? stat_buf.st_size : -1;
    assert(fsz > 0);
    
    unsigned char* ttfdata = new unsigned char[fsz];
    fread(ttfdata, 1, fsz, f);
    
    unsigned char *font_bmp = new unsigned char[c_texsz * c_texsz];
    
    font->render_data = new stbtt_bakedchar[96];
    
    stbtt_BakeFontBitmap(ttfdata, 0,
                         font->realsize,    // height
                         font_bmp, c_texsz, c_texsz,
                         32,96,             // range to bake
                         static_cast<stbtt_bakedchar*>(font->render_data));
    delete [] ttfdata;
    
    font->data = new GLuint;
    glGenTextures(1, static_cast<GLuint*>(font->data));
    glBindTexture(GL_TEXTURE_2D, *static_cast<GLuint*>(font->data));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                 c_texsz,c_texsz, 0,
                 GL_ALPHA, GL_UNSIGNED_BYTE,
                 font_bmp);
    delete [] font_bmp;
}

void OpenGL::FreeFont(Gwk::Font* font)
{
    // TODO - unbind texture
    delete [] static_cast<GLuint*>(font->data);
    delete [] static_cast<stbtt_bakedchar*>(font->render_data);
}

void OpenGL::RenderText(Gwk::Font* font, Gwk::Point pos,
                        const Gwk::String& text)
{
    if (font->facename.empty())
        return;
    
    if (!font->data)
        LoadFont(font);
    
    Texture tex;
    tex.data = font->data;
    
    float x = pos.x, y = pos.y;
    const char *pc = text.c_str();
    size_t slen = text.length();
    
    const float height = font->realsize / c_pixToPoints;
    while (slen > 0)
    {
        if (*pc >= 32 && *pc <= 127)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(static_cast<stbtt_bakedchar*>(font->render_data),
                               c_texsz,c_texsz,
                               *pc - 32,
                               &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

            Rect r(q.x0, height + q.y0, q.x1 - q.x0, q.y1 - q.y0);
            DrawTexturedRect(&tex, r, q.s0,q.t0, q.s1,q.t1);
        }
        ++pc, --slen;
    }
}

Gwk::Point OpenGL::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    Point sz(0, font->realsize * c_pixToPoints);

    if (font->facename.empty())
        return sz;
    
    if (!font->data)
        LoadFont(font);

    float x = 0.f, y = 0.f;
    const char *pc = text.c_str();
    size_t slen = text.length();
    
    while (slen > 0)
    {
        if (*pc >= 32 && *pc <= 127)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(static_cast<stbtt_bakedchar*>(font->render_data),
                               c_texsz,c_texsz,
                               *pc - 32,
                               &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9
            
            sz.x = q.x1;
            sz.y = std::max(sz.y, int(q.y1 - q.y0));
        }
        ++pc, --slen;
    }
    
    return sz;
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

}
}
