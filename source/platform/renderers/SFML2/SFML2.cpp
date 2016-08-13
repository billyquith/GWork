/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/BaseRender.h>
#include <Gwork/Renderers/SFML2.h>
#include <Gwork/Platform.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

#include <cmath>
#include <unistd.h>

struct TextureData
{
    TextureData(sf::Image* img) : texture(nullptr), image(img) {}

    TextureData(sf::Texture* text) : texture(text), image(nullptr) {}

    ~TextureData()
    {
        delete texture;
        delete image;
    }

    sf::Texture *texture;
    sf::Image *image;
};


Gwk::Renderer::SFML2::SFML2(sf::RenderTarget& target, const String& resDir)
    :   m_target(target)
    ,   m_renderStates(sf::RenderStates::Default)
    ,   m_height(m_target.getSize().y)
{
    m_buffer.setPrimitiveType(sf::Triangles);
    m_renderStates.blendMode = sf::BlendAlpha;
    
    m_resourceDir = Platform::GetExecutableDir() + resDir;
}

Gwk::Renderer::SFML2::~SFML2()
{
}

void Gwk::Renderer::SFML2::Begin()
{
    m_originalView = m_target.getView();
    
    sf::FloatRect vrect;
    vrect.left = 0;
    vrect.top = 0;
    vrect.width = m_target.getSize().x;
    vrect.height = m_height = m_target.getSize().y;
    
    sf::FloatRect vprect;
    vprect.left = 0;
    vprect.top = 0;
    vprect.width = 1.0f;
    vprect.height = 1.0f;
    
    sf::View view(vrect);
    view.setViewport(vprect);
    m_target.setView(view);
}

void Gwk::Renderer::SFML2::End()
{
    m_target.setView(m_originalView);
}

void Gwk::Renderer::SFML2::StartClip()
{
    Flush();

    Gwk::Rect rect = ClipRegion();
    float x = rect.x-1, y = rect.y, w = rect.w+1, h = rect.h+1;

    // OpenGL's coords are from the bottom left
    // so we need to translate them here.
    y = m_height-(y+h);

    const float scale = Scale();

    glEnable(GL_SCISSOR_TEST);
    glScissor(x*scale, y*scale, w*scale, h*scale);
}

void Gwk::Renderer::SFML2::EndClip()
{
    Flush();
    glDisable(GL_SCISSOR_TEST);
}

void Gwk::Renderer::SFML2::SetDrawColor(Gwk::Color color)
{
    m_color = sf::Color(color.r, color.g, color.b, color.a);
}

void Gwk::Renderer::SFML2::DrawPixel(int x, int y)
{
    EnsurePrimitiveType(sf::Points);
    EnsureTexture(nullptr);
    Translate(x, y);
    AddVert(x, y+1);
}

void Gwk::Renderer::SFML2::DrawLinedRect(Gwk::Rect rect)
{
    EnsurePrimitiveType(sf::Lines);
    EnsureTexture(nullptr);

    Translate(rect);

    //   (x,y) ---------- (x+w, y)
    //         |        |
    // (x,y+h) ---------- (x+w,y+h)

    AddVert(rect.x, rect.y);
    AddVert(rect.x+rect.w, rect.y);

    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x+rect.w, rect.y+rect.h);

    AddVert(rect.x+rect.w, rect.y+rect.h);
    AddVert(rect.x, rect.y+rect.h);

    AddVert(rect.x, rect.y+rect.h);
    AddVert(rect.x, rect.y);
}

void Gwk::Renderer::SFML2::DrawFilledRect(Gwk::Rect rect)
{
    EnsurePrimitiveType(sf::Triangles);
    EnsureTexture(nullptr);

    Translate(rect);

    AddVert(rect.x, rect.y);
    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x, rect.y+rect.h);

    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x+rect.w, rect.y+rect.h);
    AddVert(rect.x, rect.y+rect.h);
}

void Gwk::Renderer::SFML2::DrawShavedCornerRect(Gwk::Rect rect, bool bSlight)
{
    // TODO: Implement this
    Base::DrawShavedCornerRect(rect, bSlight);
}

void Gwk::Renderer::SFML2::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect, float u1,
                                            float v1, float u2, float v2)
{
    TextureData* data = reinterpret_cast<TextureData*>(texture->data);

    // Missing image, not loaded properly?
    if (!data)
        return DrawMissingImage(rect);

    const sf::Texture* tex = data->texture;

    EnsurePrimitiveType(sf::Triangles);
    EnsureTexture(tex);

    Translate(rect);

    AddVert(rect.x, rect.y, u1, v1);
    AddVert(rect.x+rect.w, rect.y, u2, v1);
    AddVert(rect.x, rect.y+rect.h,   u1, v2);

    AddVert(rect.x+rect.w, rect.y, u2, v1);
    AddVert(rect.x+rect.w, rect.y+rect.h, u2, v2);
    AddVert(rect.x, rect.y+rect.h, u1, v2);
}

void Gwk::Renderer::SFML2::LoadFont(Gwk::Font* font)
{
    font->realsize = font->size*Scale();

    sf::Font* sfFont = new sf::Font();

    if (!sfFont->loadFromFile(m_resourceDir + font->facename))
    {
        // Ideally here we should be setting the font to a system
        // default font here.
        delete sfFont;
        sfFont = nullptr; // SFML 2 doesn't have a default font anymore
    }

    font->data = sfFont;
}

void Gwk::Renderer::SFML2::FreeFont(Gwk::Font* font)
{
    if (!font->data)
        return;

    sf::Font* sfFont = reinterpret_cast<sf::Font*>(font->data);
    delete sfFont;
    font->data = nullptr;
}

void Gwk::Renderer::SFML2::RenderText(Gwk::Font* font, Gwk::Point pos,
                                      const Gwk::String& text)
{
    Flush();

    Translate(pos.x, pos.y);

    // If the font doesn't exist, or the font size should be changed
    if (!font->data || std::abs(font->realsize-font->size*Scale()) > 2)
    {
        FreeFont(font);
        LoadFont(font);
    }

    const sf::Font* sFFont = reinterpret_cast<sf::Font*>(font->data);

    sf::Text sfStr;
    sfStr.setString(text);
    sfStr.setFont(*sFFont);
    sfStr.move(pos.x, pos.y);
    sfStr.setCharacterSize(font->realsize);
    sfStr.setFillColor(m_color);
    m_target.draw(sfStr);
}

Gwk::Point Gwk::Renderer::SFML2::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    // If the font doesn't exist, or the font size should be changed
    if (!font->data || std::abs(font->realsize-font->size*Scale()) > 2)
    {
        FreeFont(font);
        LoadFont(font);
    }

    const sf::Font* sFFont = reinterpret_cast<sf::Font*>(font->data);

    sf::Text sfStr;
    sfStr.setString(text);
    sfStr.setFont(*sFFont);
    sfStr.setScale(Scale(), Scale());
    sfStr.setCharacterSize(font->realsize);
    sf::FloatRect sz = sfStr.getLocalBounds();
    return Gwk::Point(sz.left+sz.width, sz.top+sz.height);
}

void Gwk::Renderer::SFML2::LoadTexture(Gwk::Texture* texture)
{
    if (!texture)
        return;

    if (texture->data)
        FreeTexture(texture);
    
    sf::Texture* tex = new sf::Texture();
    tex->setSmooth(true);
    if (!tex->loadFromFile(m_resourceDir + texture->name))
    {
        delete tex;
        texture->failed = true;
        return;
    }

    texture->height = tex->getSize().x;
    texture->width = tex->getSize().y;
    texture->data = new TextureData(tex);
}

void Gwk::Renderer::SFML2::FreeTexture(Gwk::Texture* texture)
{
    TextureData* data = reinterpret_cast<TextureData*>(texture->data);

    delete data;

    texture->data = nullptr;
}

Gwk::Color Gwk::Renderer::SFML2::PixelColour(Gwk::Texture* texture, unsigned int x,
                                             unsigned int y, const Gwk::Color& col_default)
{
    TextureData* data = static_cast<TextureData*>(texture->data);

    if (!data->texture && !data->image)
        return col_default;

    if (!data->image)
    {
        sf::Image copy = data->texture->copyToImage();
        data->image = new sf::Image(copy);
    }

    const sf::Color col = data->image->getPixel(x, y);
    return Gwk::Color(col.r, col.g, col.b, col.a);
}
