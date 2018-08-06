/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
#ifdef _WIN32
#include <Windows.h>
#endif
# include <GL/gl.h>
#endif

#include <cmath>
#if defined(__unix__) || \
     defined(__linux__) || defined(__gnu__linux__) || \
    (defined (__APPLE__) && defined(__MACH__))
#include <unistd.h>
#endif

using namespace Gwk;

Font::Status Gwk::Renderer::SFML2::LoadFont(const Gwk::Font& font)
{
    FreeFont(font);
    m_lastFont = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Font, font.facename);

    std::unique_ptr<sf::Font> sfFont{new sf::Font()};

    if (sfFont->loadFromFile(filename))
    {
        SFMLFontData fontData;
        fontData.font = std::move(sfFont);

        m_lastFont = &(*m_fonts.insert(std::make_pair(font, std::move(fontData))).first);
        return Font::Status::Loaded;
    }
    else
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", filename.c_str());
        return Font::Status::ErrorFileNotFound;
    }
}

void Gwk::Renderer::SFML2::FreeFont(const Gwk::Font& font)
{
    if (m_lastFont != nullptr && m_lastFont->first == font)
        m_lastFont = nullptr;

    m_fonts.erase(font); // calls SFMLFontData destructor
}

bool Gwk::Renderer::SFML2::EnsureFont(const Font& font)
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

Texture::Status Gwk::Renderer::SFML2::LoadTexture(const Gwk::Texture& texture)
{
    FreeTexture(texture);
    m_lastTexture = nullptr;

    sf::Texture* tex = new sf::Texture();
    tex->setSmooth(true);

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

    if (tex->loadFromFile(filename))
    {
        SFMLTextureData texData;
        texData.height = tex->getSize().x;
        texData.width = tex->getSize().y;
        texData.texture = std::unique_ptr<sf::Texture>(tex);
        if (texture.readable)
        {
            texData.image = std::unique_ptr<sf::Image>(new sf::Image(texData.texture->copyToImage()));
            texData.readable = true;
        }

        m_lastTexture = &(*m_textures.insert(std::make_pair(texture, std::move(texData))).first);
        return Texture::Status::Loaded;
    }
    else
    {
        Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        delete tex;
        return Texture::Status::ErrorFileNotFound;
    }
}

void Gwk::Renderer::SFML2::FreeTexture(const Gwk::Texture& texture)
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        m_lastTexture = nullptr;

    m_textures.erase(texture); // calls SFMLTextureData destructor
}

TextureData Gwk::Renderer::SFML2::GetTextureData(const Texture& texture) const
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

bool Gwk::Renderer::SFML2::EnsureTexture(const Gwk::Texture& texture)
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


Gwk::Renderer::SFML2::SFML2(ResourcePaths& paths, sf::RenderTarget& target)
    :   Base(paths)
    ,   m_target(target)
    ,   m_renderStates(sf::RenderStates::Default)
    ,   m_height(m_target.getSize().y)
    ,   m_lastFont(nullptr)
    ,   m_lastTexture(nullptr)
{
    m_buffer.setPrimitiveType(sf::Triangles);
    m_renderStates.blendMode = sf::BlendAlpha;
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
    SetPrimitiveType(sf::Points);
    SetTexture(nullptr);
    Translate(x, y);
    AddVert(x, y+1);
}

void Gwk::Renderer::SFML2::DrawLinedRect(Gwk::Rect rect)
{
    SetPrimitiveType(sf::Lines);
    SetTexture(nullptr);

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
    SetPrimitiveType(sf::Triangles);
    SetTexture(nullptr);

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

void Gwk::Renderer::SFML2::DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect rect, float u1,
                                            float v1, float u2, float v2)
{
    if (!EnsureTexture(texture))
        return DrawMissingImage(rect);

    const SFMLTextureData& texData = m_lastTexture->second;

    SetPrimitiveType(sf::Triangles);
    SetTexture(texData.texture.get());

    Translate(rect);

    AddVert(rect.x, rect.y, u1, v1);
    AddVert(rect.x+rect.w, rect.y, u2, v1);
    AddVert(rect.x, rect.y+rect.h,   u1, v2);

    AddVert(rect.x+rect.w, rect.y, u2, v1);
    AddVert(rect.x+rect.w, rect.y+rect.h, u2, v2);
    AddVert(rect.x, rect.y+rect.h, u1, v2);
}

void Gwk::Renderer::SFML2::RenderText(const Gwk::Font& font, Gwk::Point pos,
                                      const Gwk::String& text)
{
    Flush();

    if (!EnsureFont(font))
        return;

    SFMLFontData& fontData = m_lastFont->second;

    Translate(pos.x, pos.y);

    sf::Text sfStr;
    sfStr.setString(text);
    sfStr.setFont(*fontData.font);
    sfStr.move(pos.x, pos.y);
    sfStr.setCharacterSize(font.size * Scale());
    sfStr.setColor(m_color);
    m_target.draw(sfStr);
}

Gwk::Point Gwk::Renderer::SFML2::MeasureText(const Gwk::Font& font, const Gwk::String& text)
{
    if (!EnsureFont(font))
        return Gwk::Point(0, 0);

    SFMLFontData& fontData = m_lastFont->second;

    sf::Text sfStr;
    sfStr.setString(text);
    sfStr.setFont(*fontData.font);
    sfStr.setScale(Scale(), Scale());
    sfStr.setCharacterSize(font.size * Scale());
    sf::FloatRect sz = sfStr.getLocalBounds();
    return Gwk::Point(sz.left+sz.width, sz.top+sz.height);
}

Gwk::Color Gwk::Renderer::SFML2::PixelColor(const Gwk::Texture& texture, unsigned int x,
                                             unsigned int y, const Gwk::Color& col_default)
{

    if (!EnsureTexture(texture))
        return col_default;

    SFMLTextureData& texData = m_lastTexture->second;

    if (!texData.texture)
        return col_default;

    if (!texData.readable)
    {
        texData.image = std::unique_ptr<sf::Image>(new sf::Image(texData.texture->copyToImage()));
        texData.readable = true;
    }

    const sf::Color col = texData.image->getPixel(x, y);
    return Gwk::Color(col.r, col.g, col.b, col.a);
}
