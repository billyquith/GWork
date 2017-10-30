/*
 *  Software Renderer for Gwork.
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/Software.h>

namespace Gwk
{
namespace Renderer
{
    
//-------------------------------------------------------------------------------

Font::Status SoftwareResourceLoader::LoadFont(Font& font)
{
    const String fontFile = m_paths.GetPath(ResourcePaths::Type::Font, font.facename);
    
    return font.status;
}

void SoftwareResourceLoader::FreeFont(Gwk::Font& font)
{
    if (font.status == Font::Status::Loaded)
    {
        font.status = Font::Status::Unloaded;
    }
}

Texture::Status SoftwareResourceLoader::LoadTexture(Texture& texture)
{
    if (texture.IsLoaded())
        FreeTexture(texture);
    
    const String texFile = m_paths.GetPath(ResourcePaths::Type::Texture, texture.name);

    return texture.status;
}

void SoftwareResourceLoader::FreeTexture(Texture& texture)
{
    if (texture.IsLoaded())
    {
        texture.status = Texture::Status::Unloaded;
    }
}

//-------------------------------------------------------------------------------
    
Software::Software(ResourceLoader& loader, PixelBuffer& pbuff)
    :   Base(loader)
    ,   m_pixbuf(&pbuff)
{
}

Software::~Software()
{
}

void Software::SetDrawColor(Gwk::Color color)
{
    m_color = color;
}

void Software::RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return;    
}

Gwk::Point Software::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return Gwk::Point(0, 0);

    return Point(0,0);
}

void Software::StartClip()
{
    const Gwk::Rect &rect = ClipRegion();
}

void Software::EndClip()
{
}

void Software::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                            float u1, float v1, float u2, float v2)
{
}

Gwk::Color Software::PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                              const Gwk::Color& col_default)
{
    Gwk::Color col;
    return col;
}

void Software::DrawFilledRect(Gwk::Rect rect)
{
    Translate(rect);
    
    // const SDL_Rect srect = { rect.x, rect.y, rect.w, rect.h };
    // SDL_RenderFillRect(m_renderer, &srect);
}

void Software::DrawLinedRect(Gwk::Rect rect)
{
    Translate(rect);
    
    // const SDL_Rect srect = { rect.x, rect.y, rect.w, rect.h };
    // SDL_RenderDrawRect(m_renderer, &srect);
}

bool Software::BeginContext(Gwk::WindowProvider* )
{
    // SDL_RenderClear(m_renderer);
    // SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    return true;
}

bool Software::EndContext(Gwk::WindowProvider* window)
{
    return true;
}

bool Software::PresentContext(Gwk::WindowProvider* window)
{
    // SDL_RenderPresent(m_renderer);
    return true;
}

} // Renderer
} // Gwork
