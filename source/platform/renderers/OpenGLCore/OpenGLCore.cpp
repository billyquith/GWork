/*
 *  Gwork - OpenGL Core renderer.
 *  Copyright (c) 2013-18 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/OpenGLCore.h>
#if defined(__APPLE__)
#   include <GL/glew.h>
#   include <OpenGL/gl.h>
#else
#   include <GL/glew.h>
#   include <GL/gl.h>
#endif
#include <Gwork/PlatformTypes.h>
#include <Gwork/WindowProvider.h>
#include <Gwork/PlatformCommon.h>
#include <Gwork/Utility.h>

#if defined(_WIN32)
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#   undef min
#   undef max
#   define CREATE_NATIVE_CONTEXT 1
#else
#   define CREATE_NATIVE_CONTEXT 0
#endif

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
#include <iostream>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Gwk {
namespace Renderer {

// See "Font Size in Pixels or Points" in "stb_truetype.h"
static constexpr float c_pointsToPixels = 1.333f;
// Arbitrary size chosen for texture cache target.
static constexpr int c_texsz = 800; // Texture size too small for wchar_t characters but stbtt_BakeFontBitmap crashes on larger sizes


OpenGLCore::GLTextureData::~GLTextureData()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, reinterpret_cast<GLuint*>(&texture_id));
}

Font::Status OpenGLCore::LoadFont(const Font& font)
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
        c_texsz, c_texsz, 0,
        GL_RED, GL_UNSIGNED_BYTE,
        font_bmp.get());

    fontData.width = c_texsz;
    fontData.height = c_texsz;

    m_lastFont = &(*m_fonts.insert(std::make_pair(font, std::move(fontData))).first);
    return Font::Status::Loaded;
}

void OpenGLCore::FreeFont(const Gwk::Font& font)
{
    if (m_lastFont != nullptr && m_lastFont->first == font)
        m_lastFont = nullptr;

    m_fonts.erase(font); // calls GLFontData destructor
}

bool OpenGLCore::EnsureFont(const Font& font)
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

Texture::Status OpenGLCore::LoadTexture(const Texture& texture)
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

void OpenGLCore::FreeTexture(const Gwk::Texture& texture)
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        m_lastTexture = nullptr;

    m_textures.erase(texture); // calls GLTextureData destructor
}

TextureData OpenGLCore::GetTextureData(const Texture& texture) const
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

bool OpenGLCore::EnsureTexture(const Gwk::Texture& texture)
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

OpenGLCore::OpenGLCore(ResourcePaths& paths, const Rect& viewRect)
    :   Base(paths)
    ,   m_context(nullptr)
    ,   m_viewRect(viewRect)
    ,   m_current_texture(0)
    ,   m_vertices(1024)
    ,   m_lastFont(nullptr)
    ,   m_lastTexture(nullptr)
{
}

OpenGLCore::~OpenGLCore()
{
}

void checkErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void OpenGLCore::Init()
{
    m_projectionMatrix = glm::ortho(
        static_cast<float>(m_viewRect.x),
        static_cast<float>(m_viewRect.x + m_viewRect.w),
        static_cast<float>(m_viewRect.y),
        static_cast<float>(m_viewRect.y + m_viewRect.h),
        -1.0f,
        1.0f
    );

    // Creating buffers
    glCreateVertexArrays(1, &m_vao);
    glCreateBuffers(1, &m_vbo);

    // Loading shaders

    // Loading shaders

    auto vertexSource = R"(
#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec4 inColor;

out vec2 TexCoords;
out vec4 VertexColor;
flat out int ShaderType;

uniform mat4 projection;
void main()
{
    ShaderType = int(inPosition.z);
    gl_Position = projection * vec4(inPosition.xy, 0.0f, 1.0f);
    TexCoords = inTexCoords;
    VertexColor = inColor;
})";
    auto fragmentTexturedSource = R"(
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec4 VertexColor;
flat in int ShaderType;

uniform sampler2D texture1;
void main()
{
    if (ShaderType == 0)
        FragColor = VertexColor;
    else if (ShaderType == 1)
        FragColor = texture(texture1, TexCoords) * VertexColor;
    else if (ShaderType == 2)
    {
        float color = texture(texture1, TexCoords).r;
        FragColor = vec4(VertexColor.rgb, color);
    }
})";

    m_program = loadShaders(vertexSource, fragmentTexturedSource);

    glViewport(m_viewRect.x, m_viewRect.y, m_viewRect.w, m_viewRect.h);
}

void OpenGLCore::Begin()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
}

void OpenGLCore::End()
{
    Flush();
}

void OpenGLCore::Flush()
{
    if (m_vertices.empty())
    {
        return;
    }

    // Binding VAO
    glBindVertexArray(m_vao);

    // Binding VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Loading data into VBO
    glNamedBufferData(
        m_vbo,
        m_vertices.size() * sizeof(Vertex),
        m_vertices.data(),
        GL_DYNAMIC_DRAW
    );

    // Binding VBO to VAO
    glVertexArrayVertexBuffer(
        m_vao,
        0,
        m_vbo,
        0,
        sizeof(Vertex)
    );

    // Enabling attributes
    glEnableVertexArrayAttrib(m_vao, 0); // Position
    glEnableVertexArrayAttrib(m_vao, 1); // UV
    glEnableVertexArrayAttrib(m_vao, 2); // Color

    // Setting format
    glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE,static_cast<GLuint>(offsetof(Vertex, pos)));
    glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, uv)));
    glVertexArrayAttribFormat(m_vao, 2, 4, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, color)));

    // Binding vertex array to buffer
    glVertexArrayAttribBinding(m_vao, 0, 0);
    glVertexArrayAttribBinding(m_vao, 1, 0);
    glVertexArrayAttribBinding(m_vao, 2, 0);

    // Program
    glUseProgram(m_program);

    // Loading matricies
    glProgramUniformMatrix4fv(
        m_program,
        glGetUniformLocation(m_program, "projection"),
        1,
        GL_FALSE,
        glm::value_ptr(m_projectionMatrix)
    );

    // Drawing
    glDrawArrays(
        GL_TRIANGLES,
        0,
        static_cast<GLsizei>(m_vertices.size())
    );

    // Unbinding
    glBindVertexArray(0);
    glUseProgram(0);

    // Clearing
    m_activeProgram = 0;
    m_vertices.clear();
    glFlush();
}

void OpenGLCore::AddVert(int x, int y, float u, float v)
{
    // OpenGL origin is bottom-left. Gwork origin is top-left.

    Vertex vertex;
    vertex.pos.x = x;
    vertex.pos.y = m_viewRect.h - y;
    vertex.pos.z = m_activeProgram;

    vertex.uv.x = u;
    vertex.uv.y = v;

    vertex.color = glm::vec4(
        m_color.r / 255.0f,
        m_color.g / 255.0f,
        m_color.b / 255.0f,
        m_color.a / 255.0f
    );

    m_vertices.emplace_back(vertex);
}

void OpenGLCore::SetTexture(unsigned int texture)
{
    if (texture == 0 && !m_textures_on)
        return;

    if (texture == 0)
    {
        m_textures_on = false;
    }
    else
    {
        m_textures_on = true;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    m_current_texture = texture;
}

void OpenGLCore::DrawFilledRect(Gwk::Rect rect)
{
    m_activeProgram = 0;

    Translate(rect);
    AddVert(rect.x, rect.y);
    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x, rect.y+rect.h);
    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x+rect.w, rect.y+rect.h);
    AddVert(rect.x, rect.y+rect.h);
}

void OpenGLCore::SetDrawColor(Gwk::Color color)
{
//            glColor4ubv((GLubyte*)&color);
    m_color = color;
}

void OpenGLCore::StartClip()
{
    Flush();
    Gwk::Rect rect = ClipRegion();

    rect.y = m_viewRect.h - (rect.y + rect.h);

    glScissor(rect.x * Scale(), rect.y * Scale(),
              rect.w * Scale(), rect.h * Scale());
    glEnable(GL_SCISSOR_TEST);
}

void OpenGLCore::EndClip()
{
    Flush();
    glDisable(GL_SCISSOR_TEST);
}

void OpenGLCore::DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect rect,
    float u1, float v1, float u2, float v2)
{
    if (!EnsureTexture(texture))
        return DrawMissingImage(rect);

    GLTextureData& texData = m_lastTexture->second;

    Translate(rect);

    if (!m_current_texture || texData.texture_id != m_current_texture)
    {
        Flush();
        SetTexture(texData.texture_id);
    }

    m_activeProgram = 1;
    AddVert(rect.x, rect.y, u1, v1);
    AddVert(rect.x + rect.w, rect.y, u2, v1);
    AddVert(rect.x, rect.y + rect.h, u1, v2);
    AddVert(rect.x + rect.w, rect.y, u2, v1);
    AddVert(rect.x + rect.w, rect.y + rect.h, u2, v2);
    AddVert(rect.x, rect.y + rect.h, u1, v2);
}

Gwk::Color OpenGLCore::PixelColor(const Gwk::Texture& texture, unsigned int x, unsigned int y,
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

void OpenGLCore::RenderText(const Gwk::Font& font, Gwk::Point pos,
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
    m_activeProgram = 2;
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
            fontData.width, fontData.height,
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

Gwk::Point OpenGLCore::MeasureText(const Gwk::Font& font, const Gwk::String& text)
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
            fontData.width, fontData.height,
            c,
            &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

        sz.x = q.x1;
        sz.y = std::max(sz.y, int((q.y1 - q.y0) * c_pointsToPixels));
    }

    return sz;
}

bool OpenGLCore::InitializeContext(Gwk::WindowProvider* window)
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

bool OpenGLCore::ShutdownContext(Gwk::WindowProvider* window)
{

#if CREATE_NATIVE_CONTEXT
    wglDeleteContext((HGLRC)m_context);
    return true;
#endif
    return false;
}

bool OpenGLCore::PresentContext(Gwk::WindowProvider* window)
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

bool OpenGLCore::ResizedContext(Gwk::WindowProvider* window, int w, int h)
{
    std::cout << "ResizedContext w=" << w << ", h=" << h << std::endl;
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

bool OpenGLCore::BeginContext(Gwk::WindowProvider* window)
{
    std::cout << "BeginContext" << std::endl;
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    return true;
}

bool OpenGLCore::EndContext(Gwk::WindowProvider* window)
{
    std::cout << "EndContext" << std::endl;
    return true;
}

uint32_t OpenGLCore::loadShaders(const char *vertex, const char *fragment)
{
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Binding shaders with sources
    glShaderSource(
        vertexShader,
        1,
        reinterpret_cast<const GLchar* const*>(&vertex),
        nullptr
    );
    glShaderSource(
        fragmentShader,
        1,
        reinterpret_cast<const GLchar* const*>(&fragment),
        nullptr
    );

    // Compiling shaders
    glCompileShader(vertexShader);

    checkErrors(vertexShader, "VERTEX_SHADER");

    glCompileShader(fragmentShader);

    checkErrors(fragmentShader, "FRAGMENT_SHADER");

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    checkErrors(program, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

} // namespace Renderer
} // namespace Gwk
