

/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */


#include <GL/glew.h>
#include <GL/gl.h>
#include <Gwork/Renderers/OpenGLCore.h>
#include <Gwork/PlatformTypes.h>
#include <Gwork/WindowProvider.h>
#include <Gwork/PlatformCommon.h>

#ifdef _WIN32
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

// todo: remove lib if not GLFW driven
//#include <GLFW/glfw3.h>
//#include <GL/glew.h>

namespace Gwk
{
    namespace Renderer
    {

// See "Font Size in Pixels or Points" in "stb_truetype.h"
        static constexpr float c_pointsToPixels = 1.333f;
// Arbitrary size chosen for texture cache target.
        static constexpr int c_texsz = 256;

        Font::Status OpenGLResourceLoader::LoadFont(Font& font)
        {
            const String filename = m_paths.GetPath(ResourcePaths::Type::Font, font.facename);

            FILE* f = fopen(filename.c_str(), "rb");
            if (!f)
            {
                Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", filename.c_str());
                font.data = nullptr;
                font.status = Font::Status::ErrorFileNotFound;
                return font.status;
            }

            struct stat stat_buf;
            const int rc = stat(filename.c_str(), &stat_buf);
            const size_t fsz = rc == 0 ? stat_buf.st_size : -1;
            assert(fsz > 0);

            auto* ttfdata = new unsigned char[fsz];
            fread(ttfdata, 1, fsz, f);
            fclose(f);

            auto*font_bmp = new unsigned char[c_texsz * c_texsz];

            font.render_data = new stbtt_bakedchar[96];

            stbtt_BakeFontBitmap(ttfdata, 0,
                                 font.realsize * c_pointsToPixels, // height
                                 font_bmp,
                                 c_texsz, c_texsz,
                                 32,96,             // range to bake
                                 static_cast<stbtt_bakedchar*>(font.render_data));
            delete [] ttfdata;

            font.data = new GLuint;
            glGenTextures(1, static_cast<GLuint*>(font.data));
            glBindTexture(GL_TEXTURE_2D, *static_cast<GLuint*>(font.data));
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                         c_texsz,c_texsz, 0,
                         GL_RED, GL_UNSIGNED_BYTE,
                         font_bmp);
            delete [] font_bmp;

            glBindTexture(GL_TEXTURE_2D, 0);
            font.status = Font::Status::Loaded;

            return font.status;
        }

        void OpenGLResourceLoader::FreeFont(Gwk::Font& font)
        {
            if (font.IsLoaded())
            {
                // TODO - unbind texture
                delete [] static_cast<GLuint*>(font.data);
                delete [] static_cast<stbtt_bakedchar*>(font.render_data);

                font.status = Font::Status::Unloaded;
            }
        }

        Texture::Status OpenGLResourceLoader::LoadTexture(Texture& texture)
        {
            if (texture.IsLoaded())
                FreeTexture(texture);

            const String filename = m_paths.GetPath(ResourcePaths::Type::Texture, texture.name);

            int x,y,n;
            unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 4);

            // Image failed to load..
            if (!data)
            {
                Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
                texture.status = Texture::Status::ErrorFileNotFound;
                return texture.status;
            }

            // Create a little texture pointer..
            auto* pglTexture = new GLuint;

            texture.data = pglTexture;
            texture.width = x;
            texture.height = y;

            // Create the opengl texture
            glGenTextures(1, pglTexture);
            glBindTexture(GL_TEXTURE_2D, *pglTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            GLenum format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, format,
                         GL_UNSIGNED_BYTE, (const GLvoid*)data);

            stbi_image_free(data);

            glBindTexture(GL_TEXTURE_2D, *pglTexture);

            return texture.status = Texture::Status::Loaded;
        }

        void OpenGLResourceLoader::FreeTexture(Texture& texture)
        {
            if (texture.IsLoaded())
            {
                GLuint* tex = static_cast<GLuint*>(texture.data);

                glDeleteTextures(1, tex);
                delete tex;
                texture.data = nullptr;

                texture.status = Texture::Status::Unloaded;
            }
        }

//-------------------------------------------------------------------------------

        OpenGLCore::OpenGLCore(ResourceLoader& loader, const Rect& viewRect)
            :   Base(loader)
            ,   m_context(nullptr)
            ,   m_viewRect(viewRect)
            ,   m_currentTexture(0)
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
            std::cout
                << "Init Rect(x=" << m_viewRect.x
                         << " y=" << m_viewRect.y << " "
                         << m_viewRect.w << 'x'
                         << m_viewRect.h << ")" << std::endl;

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

            auto vertexSource = R"(
#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec4 inColor;

out vec2 TexCoords;
out vec4 VertexColor;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(inPosition, 1.0f);
    TexCoords = inTexCoords;
    VertexColor = inColor;
})";
            auto fragmentTexturedSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 VertexColor;

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords) * VertexColor;
})";

            auto fragmentFontSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 VertexColor;

uniform sampler2D texture1;

void main()
{
    float color = texture(texture1, TexCoords).r;

    FragColor = vec4(VertexColor.rgb, color);
})";

            auto fragmentSolidSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 VertexColor;

uniform sampler2D texture1;

void main()
{
    FragColor = VertexColor;
})";

            m_texturedProgram = loadShaders(vertexSource, fragmentTexturedSource);
            m_fontProgram =     loadShaders(vertexSource, fragmentFontSource);
            m_solidProgram =    loadShaders(vertexSource, fragmentSolidSource);

            glViewport(m_viewRect.x, m_viewRect.y, m_viewRect.w, m_viewRect.h);
        }

        void OpenGLCore::Begin()
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            m_zPos = 0;
        }

        void OpenGLCore::End()
        {
            Flush();
        }

        void OpenGLCore::Flush()
        {
            for (auto&& element : m_vertices)
            {
                if (element.second.empty())
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
                    element.second.size() * sizeof(Vertex),
                    element.second.data(),
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
                glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, pos)));
                glVertexArrayAttribFormat(m_vao, 1, 2, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, uv)));
                glVertexArrayAttribFormat(m_vao, 2, 4, GL_FLOAT, GL_FALSE, static_cast<GLuint>(offsetof(Vertex, color)));

                // Binding vertex array to buffer
                glVertexArrayAttribBinding(m_vao, 0, 0);
                glVertexArrayAttribBinding(m_vao, 1, 0);
                glVertexArrayAttribBinding(m_vao, 2, 0);

                // Program
                glUseProgram(element.first.second);

                // Loading matricies
                glProgramUniformMatrix4fv(
                    element.first.second,
                    glGetUniformLocation(element.first.second, "projection"),
                    1,
                    GL_FALSE,
                    glm::value_ptr(m_projectionMatrix)
                );

                if (element.first.first)
                {
                    glBindTexture(GL_TEXTURE_2D, element.first.first);
                }

                // Drawing
                glDrawArrays(
                    GL_TRIANGLES,
                    0,
                    static_cast<GLsizei>(element.second.size())
                );

                glBindTexture(GL_TEXTURE_2D, 0);

                // Unbinding
                glBindVertexArray(0);
                glUseProgram(0);

                // Clearing
                element.second.clear();
            }
        }

        void OpenGLCore::AddVert(GLuint program, int x, int y, float u, float v, GLuint texture)
        {
            // OpenGL origin is bottom-left. Gwork origin is top-left.

            Vertex vertex;
            vertex.pos.x = x;
            vertex.pos.y = m_viewRect.h - y;
            vertex.pos.z = m_zPos;

            m_zPos += 0.0001;

            vertex.uv.x = u;
            vertex.uv.y = v;

            vertex.color = glm::vec4(
                m_color.r / 255.0f,
                m_color.g / 255.0f,
                m_color.b / 255.0f,
                m_color.a / 255.0f
            );

            auto iter = m_vertices.find({texture, program});
            if (iter == m_vertices.end())
            {
                m_vertices[{texture, program}] = std::vector<Vertex>();
                iter = m_vertices.find({texture, program});
            }

            iter->second.push_back(vertex);
//            m_vertices.push_back(vertex);
        }

        void OpenGLCore::DrawFilledRect(Gwk::Rect rect)
        {
//            std::cout << "DrawFilledRect Rect(x=" << rect.x << " y=" << rect.y << " " << rect.w << 'x' << rect.h << ")" << std::endl;

            Translate(rect);
            AddVert(m_solidProgram, rect.x, rect.y);
            AddVert(m_solidProgram, rect.x+rect.w, rect.y);
            AddVert(m_solidProgram, rect.x, rect.y+rect.h);
            AddVert(m_solidProgram, rect.x+rect.w, rect.y);
            AddVert(m_solidProgram, rect.x+rect.w, rect.y+rect.h);
            AddVert(m_solidProgram, rect.x, rect.y+rect.h);
        }

        void OpenGLCore::SetDrawColor(Gwk::Color color)
        {
//            glColor4ubv((GLubyte*)&color);
            m_color = color;
        }

        void OpenGLCore::StartClip()
        {
//            Flush();
            Gwk::Rect rect = ClipRegion();

            rect.y = m_viewRect.h - (rect.y + rect.h);

            glScissor(rect.x * Scale(), rect.y * Scale(),
                      rect.w * Scale(), rect.h * Scale());
            glEnable(GL_SCISSOR_TEST);
        }

        void OpenGLCore::EndClip()
        {
//            Flush();
            glDisable(GL_SCISSOR_TEST);
        }

        void OpenGLCore::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                                          float u1, float v1, float u2, float v2)
        {
            auto* tex = (GLuint*)texture->data;

            // Missing image, not loaded properly?
            if (!tex)
                return DrawMissingImage(rect);

            Translate(rect);

//            if (m_currentTexture != *tex)
//            {
//                Flush();
//                glActiveTexture(GL_TEXTURE0);
//                glBindTexture(GL_TEXTURE_2D, *tex);
//                m_currentTexture = *tex;
//            }

            AddVert(m_texturedProgram, rect.x,        rect.y,        u1, v1, *tex);
            AddVert(m_texturedProgram, rect.x+rect.w, rect.y,        u2, v1, *tex);
            AddVert(m_texturedProgram, rect.x,        rect.y+rect.h, u1, v2, *tex);
            AddVert(m_texturedProgram, rect.x+rect.w, rect.y,        u2, v1, *tex);
            AddVert(m_texturedProgram, rect.x+rect.w, rect.y+rect.h, u2, v2, *tex);
            AddVert(m_texturedProgram, rect.x,        rect.y+rect.h, u1, v2, *tex);
        }


        void OpenGLCore::DrawTexturedFontRect(Gwk::Texture* texture, Gwk::Rect rect,
                                              float u1, float v1, float u2, float v2)
        {
            auto* tex = (GLuint*)texture->data;

            // Missing image, not loaded properly?
            if (!tex)
                return DrawMissingImage(rect);

            Translate(rect);

            AddVert(m_fontProgram, rect.x,        rect.y,        u1, v1, *tex);
            AddVert(m_fontProgram, rect.x+rect.w, rect.y,        u2, v1, *tex);
            AddVert(m_fontProgram, rect.x,        rect.y+rect.h, u1, v2, *tex);
            AddVert(m_fontProgram, rect.x+rect.w, rect.y,        u2, v1, *tex);
            AddVert(m_fontProgram, rect.x+rect.w, rect.y+rect.h, u2, v2, *tex);
            AddVert(m_fontProgram, rect.x,        rect.y+rect.h, u1, v2, *tex);
        }

        Gwk::Color OpenGLCore::PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                      const Gwk::Color& col_default)
        {
            auto* tex = (GLuint*)texture->data;

            if (!tex)
                return col_default;

            unsigned int iPixelSize = sizeof(unsigned char)*4;
            glBindTexture(GL_TEXTURE_2D, *tex);
            auto* data = new unsigned char[iPixelSize * texture->width * texture->height];
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            unsigned int iOffset = (y*texture->width+x)*4;

            Color c(data[0+iOffset], data[1+iOffset], data[2+iOffset], data[3+iOffset]);

            // Retrieving the entire texture for a single pixel read
            // is kind of a waste - maybe cache this pointer in the texture
            // data and then release later on? It's never called during runtime
            // - only during initialization.
            delete[] data;
            glBindTexture(GL_TEXTURE_2D, 0);

            return c;
        }

        void OpenGLCore::RenderText(Gwk::Font* font, Gwk::Point pos,
                                const Gwk::String& text)
        {

            Texture tex;
            tex.data = font->data;

            float x = pos.x, y = pos.y;
            const char *pc = text.c_str();
            size_t slen = text.length();

            // Height of font, allowing for descenders, because baseline is bottom of capitals.
            const float height = font->realsize * c_pointsToPixels * 0.8f;

            while (slen > 0)
            {
                if (*pc >= 32 && *pc <= 127)
                {
                    stbtt_aligned_quad q;
                    stbtt_GetBakedQuad(static_cast<stbtt_bakedchar*>(font->render_data),
                                       c_texsz,c_texsz,
                                       *pc - 32,
                                       &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

                    m_isFontRendering = true;
                    DrawTexturedFontRect(
                        &tex,
                        Rect(
                            static_cast<int>(q.x0),
                            static_cast<int>(q.y0 + height),
                            static_cast<int>(q.x1 - q.x0),
                            static_cast<int>(q.y1 - q.y0)
                        ),
                        q.s0,
                        q.t0,
                        q.s1,
                        q.t1
                    );
                }
                ++pc;
                --slen;
            }
        }

        Gwk::Point OpenGLCore::MeasureText(Gwk::Font* font, const Gwk::String& text)
        {

            if (!EnsureFont(*font))
                return {0, 0};

            Point sz(0, static_cast<int>(font->realsize * c_pointsToPixels));

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

                    sz.x = static_cast<int>(q.x1);
                    sz.y = std::max(sz.y, int((q.y1 - q.y0) * c_pointsToPixels));
                }
                ++pc, --slen;
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

    }

    uint32_t Renderer::OpenGLCore::loadShaders(const char *vertex, const char *fragment)
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
}
