
#include <Gwork/Renderers/OpenGL_DebugFont.h>
#include <Gwork/Utility.h>
#include <Gwork/Font.h>
#include <Gwork/Texture.h>

#include <math.h>
#include "GL/glew.h"

#include "FontData.h"


namespace Gwk
{
namespace Renderer
{
OpenGL_DebugFont::OpenGL_DebugFont()
{
    m_fLetterSpacing = 1.0f/16.0f;
    m_fFontScale[0] = 1.5f;
    m_fFontScale[1] = 1.5f;
    m_fontTexture = nullptr;
}

void OpenGL_DebugFont::Init()
{
    CreateDebugFont();
}

OpenGL_DebugFont::~OpenGL_DebugFont()
{
    DestroyDebugFont();
}

void OpenGL_DebugFont::CreateDebugFont()
{
    if (m_fontTexture)
        return;

    m_fontTexture = new Gwk::Texture();
    // Create a little texture pointer..
    GLuint* pglTexture = new GLuint;
    // Sort out our Gwork texture
    m_fontTexture->data = pglTexture;
    m_fontTexture->width = 256;
    m_fontTexture->height = 256;
    // Create the opengl texture
    glGenTextures(1, pglTexture);
    glBindTexture(GL_TEXTURE_2D, *pglTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* texdata = new unsigned char[256*256*4];

    for (int i = 0; i < 256*256; i++)
    {
        texdata[i*4] = sGworkFontData[i];
        texdata[i*4+1] = sGworkFontData[i];
        texdata[i*4+2] = sGworkFontData[i];
        texdata[i*4+3] = sGworkFontData[i];
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_fontTexture->width, m_fontTexture->height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)texdata);
    delete[]texdata;
}

void OpenGL_DebugFont::DestroyDebugFont()
{
    if (!m_fontTexture)
        return;

    GLuint* tex = (GLuint*)m_fontTexture->data;

    if (!tex)
        return;

    glDeleteTextures(1, tex);
    delete tex;
    m_fontTexture->data = nullptr;
    delete m_fontTexture;
    m_fontTexture = nullptr;
}

void OpenGL_DebugFont::RenderText(Gwk::Font* font, Gwk::Point pos,
                                  const Gwk::String& text)
{
    float fSize = font->size*Scale();

    if (!text.length())
        return;

    float yOffset = 0.0f;

    for (unsigned int i = 0; i < text.length(); i++)
    {
        char ch = text[i];
        float curSpacing = sGworkDebugFontSpacing[ch]*m_fLetterSpacing*fSize*m_fFontScale[0];
        Gwk::Rect r(pos.x+yOffset, pos.y-fSize*0.5, (fSize*m_fFontScale[0]),
                     fSize*m_fFontScale[1]);

        if (m_fontTexture)
        {
            float uv_texcoords[8] = {0., 0., 1., 1.};

            if (ch >= 0)
            {
                float cx = (ch%16)/16.0;
                float cy = (ch/16)/16.0;
                uv_texcoords[0] = cx;
                uv_texcoords[1] = cy;
                uv_texcoords[4] = float(cx+1.0f/16.0f);
                uv_texcoords[5] = float(cy+1.0f/16.0f);
            }

            DrawTexturedRect(m_fontTexture, r, uv_texcoords[0], uv_texcoords[5],
                             uv_texcoords[4], uv_texcoords[1]);
            yOffset += curSpacing;
        }
        else
        {
            DrawFilledRect(r);
            yOffset += curSpacing;
        }
    }
}

Gwk::Point OpenGL_DebugFont::MeasureText(Gwk::Font* font,
                                          const Gwk::String& text)
{
    Gwk::Point p;
    float fSize = font->size*Scale();
    float spacing = 0.0f;

    for (unsigned int i = 0; i < text.length(); i++)
    {
        char ch = text[i];
        spacing += sGworkDebugFontSpacing[ch];
    }

    p.x = spacing*m_fLetterSpacing*fSize*m_fFontScale[0];
    p.y = font->size*Scale();
    return p;
}

}
}
