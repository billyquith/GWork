/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/OpenGL.h>
#include <Gwork/PlatformTypes.h>
#include <Gwork/WindowProvider.h>
#include <Gwork/PlatformCommon.h>
#include <Gwork/Utility.h>

#ifdef _WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef min
#   undef max
#   define CREATE_NATIVE_CONTEXT 1
#else
#   define CREATE_NATIVE_CONTEXT 0
#endif

#include <GLFW/glfw3.h>
#include <math.h>
#include <sys/stat.h>
#include <stdio.h>
#include <algorithm>
#include <fstream>

//#define STBI_ASSERT(x)  // comment in for no asserts
#define STB_IMAGE_IMPLEMENTATION
#include <Gwork/External/stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include <Gwork/External/stb_truetype.h>

namespace Gwk
{
namespace Renderer
{

// See "Font Size in Pixels or Points" in "stb_truetype.h"
static constexpr float c_pointsToPixels = 1.333f;
// Arbitrary size chosen for texture cache target.
static constexpr int c_texsz = 800; // Texture size too small for wchar_t characters but stbtt_BakeFontBitmap crashes on larger sizes

OpenGL::GLTextureData::~GLTextureData()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, reinterpret_cast<GLuint*>(&texture_id));
}

Font::Status OpenGL::LoadFont(const Font& font)
{
    FreeFont(font);
    m_lastFont = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Font, font.facename);

    std::ifstream inFile(filename, std::ifstream::in | std::ifstream::binary);

    if (!inFile.good())
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", filename.c_str());
        return Font::Status::ErrorFileNotFound;
    }

    std::streampos begin = inFile.tellg();
    inFile.seekg(0, std::ios::end);
    const size_t fsz = inFile.tellg() - begin;
    inFile.seekg(0, std::ios::beg);
    assert(fsz > 0);

    std::unique_ptr<unsigned char[]> ttfdata = std::unique_ptr<unsigned char[]>(new unsigned char[fsz]);
    inFile.read(reinterpret_cast<char*>(ttfdata.get()), fsz);
    inFile.close();

    std::unique_ptr<unsigned char[]> font_bmp = std::unique_ptr<unsigned char[]>(new unsigned char[c_texsz * c_texsz]);

    GLFontData fontData;
    const float realsize = font.size * Scale();
    fontData.baked_chars.resize(LastCharacter - BeginCharacter + 1);

    stbtt_BakeFontBitmap(ttfdata.get(), 0,
                         realsize * c_pointsToPixels, // height
                         font_bmp.get(),
                         c_texsz, c_texsz,
                         BeginCharacter, LastCharacter,             // range to bake
                         reinterpret_cast<stbtt_bakedchar*>(fontData.baked_chars.data()));
    
    glGenTextures(1, &fontData.texture_id);
    SetTexture(fontData.texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                 c_texsz,c_texsz, 0,
                 GL_ALPHA, GL_UNSIGNED_BYTE,
                 font_bmp.get());

    fontData.width = c_texsz;
    fontData.height = c_texsz;

    m_lastFont = &(*m_fonts.insert(std::make_pair(font, std::move(fontData))).first);
    return Font::Status::Loaded;
}

void OpenGL::FreeFont(const Gwk::Font& font)
{
    if (m_lastFont != nullptr && m_lastFont->first == font)
        m_lastFont = nullptr;

    m_fonts.erase(font); // calls GLFontData destructor
}

bool OpenGL::EnsureFont(const Font& font)
{
    if (m_lastFont != nullptr)
    {
        if (m_lastFont->first == font)
            return true;
    }

    // Was it loaded before?
    auto it = m_fonts.find(font);
    if (it != m_fonts.end())
    {
        m_lastFont = &(*it);
        return true;
    }

    // No, try load to it

    // LoadFont sets m_lastFont, if loaded
    return LoadFont(font) == Font::Status::Loaded;
}

Texture::Status OpenGL::LoadTexture(const Texture& texture)
{
    FreeTexture(texture);
    m_lastTexture = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

    GLTextureData texData;

    int width, height, n;
    {
        unsigned char *image = stbi_load(filename.c_str(), &width, &height, &n, 4);
        texData.m_ReadData = deleted_unique_ptr<unsigned char>(image, [](unsigned char* mem) { if (mem) stbi_image_free(mem); });
    }

    // Image failed to load..
    if (!texData.m_ReadData)
    {
        Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        return Texture::Status::ErrorFileNotFound;
    }
    

    // Create the opengl texture
    glGenTextures(1, &texData.texture_id);
    SetTexture(texData.texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLenum format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format,
                 GL_UNSIGNED_BYTE, (const GLvoid*)texData.m_ReadData.get());

    texData.readable = texture.readable;
    if (!texture.readable)
    {
        texData.m_ReadData.reset();
    }

    texData.width = width;
    texData.height = height;

    m_lastTexture = &(*m_textures.insert(std::make_pair(texture, std::move(texData))).first);
    return Texture::Status::Loaded;
}

void OpenGL::FreeTexture(const Gwk::Texture& texture)
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        m_lastTexture = nullptr;

    m_textures.erase(texture); // calls GLTextureData destructor
}

TextureData OpenGL::GetTextureData(const Texture& texture) const
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        return m_lastTexture->second;

    auto it = m_textures.find(texture);
    if (it != m_textures.cend())
    {
        return it->second;
    }
    // Texture not loaded :(
    return TextureData();
}

bool OpenGL::EnsureTexture(const Gwk::Texture& texture)
{
    if (m_lastTexture != nullptr)
    {
        if (m_lastTexture->first == texture)
            return true;
    }

    // Was it loaded before?
    auto it = m_textures.find(texture);
    if (it != m_textures.end())
    {
        m_lastTexture = &(*it);
        return true;
    }

    // No, try load to it

    // LoadTexture sets m_lastTexture, if exist
    return LoadTexture(texture) == Texture::Status::Loaded;
}

//-------------------------------------------------------------------------------

OpenGL::OpenGL(ResourcePaths& paths, const Rect& viewRect)
:   Base(paths)
,   m_viewRect(viewRect)
,   m_vertNum(0)
,   m_context(nullptr)
,   m_lastFont(nullptr)
,   m_lastTexture(nullptr)
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

    glGetBooleanv(GL_TEXTURE_2D, &m_textures_on);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&m_current_texture));
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

void OpenGL::SetTexture(unsigned int texture)
{
    if (texture == 0 && !m_textures_on)
        return;

    if (texture == 0)
    {
        glDisable(GL_TEXTURE_2D);
        m_textures_on = false;
    }
    else
    {
        glEnable(GL_TEXTURE_2D);
        m_textures_on = true;
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    m_current_texture = texture;
}

void OpenGL::DrawFilledRect(Gwk::Rect rect)
{
    if (m_textures_on)
    {
        Flush();
        SetTexture(0);
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

void OpenGL::DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect rect,
                              float u1, float v1, float u2, float v2)
{
    if (!EnsureTexture(texture))
        return DrawMissingImage(rect);

    const GLTextureData& texData = m_lastTexture->second;

    Translate(rect);

    if (!m_current_texture || texData.texture_id != m_current_texture)
    {
        Flush();
        SetTexture(texData.texture_id);
    }

    AddVert(rect.x, rect.y,             u1, v1);
    AddVert(rect.x+rect.w, rect.y,      u2, v1);
    AddVert(rect.x, rect.y+rect.h,      u1, v2);
    AddVert(rect.x+rect.w, rect.y,      u2, v1);
    AddVert(rect.x+rect.w, rect.y+rect.h, u2, v2);
    AddVert(rect.x, rect.y+rect.h,      u1, v2);
}

Gwk::Color OpenGL::PixelColor(const Gwk::Texture& texture, unsigned int x, unsigned int y,
                               const Gwk::Color& col_default)
{
    if (!EnsureTexture(texture))
        return col_default;

    GLTextureData& texData = m_lastTexture->second;

    static const unsigned int iPixelSize = sizeof(unsigned char) * 4;

    if (texData.readable)
    {
        unsigned char *pPixel = texData.m_ReadData.get() + (x + (y * static_cast<unsigned int>(texData.width))) * iPixelSize;
        return Gwk::Color(pPixel[0], pPixel[1], pPixel[2], pPixel[3]);
    }

    SetTexture(texData.texture_id);
    texData.m_ReadData = deleted_unique_ptr<unsigned char>(new unsigned char[texData.width * texData.height * iPixelSize], [](unsigned char* mem) { if (mem) delete[](mem); });
    texData.readable = true;

    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.m_ReadData.get());

    unsigned char *pPixel = texData.m_ReadData.get() + (x + (y * static_cast<unsigned int>(texData.width))) * iPixelSize;
    return Gwk::Color(pPixel[0], pPixel[1], pPixel[2], pPixel[3]);
}

void OpenGL::RenderText(const Gwk::Font& font, Gwk::Point pos,
                        const Gwk::String& text)
{
    if (!EnsureFont(font))
        return;

    GLFontData& fontData = m_lastFont->second;

    if (m_current_texture != fontData.texture_id)
    {
        Flush();
        SetTexture(fontData.texture_id);
    }
    float x = pos.x, y = pos.y;
    char* text_ptr = const_cast<char*>(text.c_str());

    // Height of font, allowing for descenders, because baseline is bottom of capitals.
    const float height = font.size * Scale() * c_pointsToPixels * 0.8f;

    while (const auto wide_char = Utility::Strings::utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;

        if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(reinterpret_cast<stbtt_bakedchar*>(fontData.baked_chars.data()),
            c_texsz, c_texsz,
            c,
            &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

        Rect rect(q.x0, q.y0 + height, q.x1 - q.x0, q.y1 - q.y0);

        Translate(rect);

        AddVert(rect.x, rect.y, q.s0, q.t0);
        AddVert(rect.x + rect.w, rect.y, q.s1, q.t0);
        AddVert(rect.x, rect.y + rect.h, q.s0, q.t1);
        AddVert(rect.x + rect.w, rect.y, q.s1, q.t0);
        AddVert(rect.x + rect.w, rect.y + rect.h, q.s1, q.t1);
        AddVert(rect.x, rect.y + rect.h, q.s0, q.t1);
    }
}

Gwk::Point OpenGL::MeasureText(const Gwk::Font& font, const Gwk::String& text)
{
    if (!EnsureFont(font))
        return Gwk::Point(0, 0);

    GLFontData& fontData = m_lastFont->second;

    Point sz(0, font.size * Scale() * c_pointsToPixels);

    float x = 0.f, y = 0.f;
    char* text_ptr = const_cast<char*>(text.c_str());

    while (const auto wide_char = Utility::Strings::utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;

        if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;


        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(reinterpret_cast<stbtt_bakedchar*>(fontData.baked_chars.data()),
            c_texsz, c_texsz,
            c,
            &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

        sz.x = q.x1;
        sz.y = std::max(sz.y, int((q.y1 - q.y0) * c_pointsToPixels));
    }

    return sz;
}

bool OpenGL::InitializeContext(Gwk::WindowProvider* window)
{
#if CREATE_NATIVE_CONTEXT
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
#if CREATE_NATIVE_CONTEXT
    wglDeleteContext((HGLRC)m_context);
    return true;
#endif
    return false;
}

bool OpenGL::PresentContext(Gwk::WindowProvider* window)
{
#if CREATE_NATIVE_CONTEXT
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
#if CREATE_NATIVE_CONTEXT
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
