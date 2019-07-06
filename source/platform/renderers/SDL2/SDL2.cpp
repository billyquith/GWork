
// SDL2 Renderer for Gwork - http://www.libsdl.org/
// Added by BQ.

#include <Gwork/BaseRender.h>
#include <Gwork/Renderers/SDL2.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Gwk
{
namespace Renderer
{

//-------------------------------------------------------------------------------

Font::Status SDL2::LoadFont(const Font& font)
{
    FreeFont(font);
    m_lastFont = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Font, font.facename);

    TTF_Font *tfont = TTF_OpenFont(filename.c_str(), font.size * Scale());
    if (tfont)
    {
        SDL2FontData fontData;
        fontData.tFont = deleted_unique_ptr<TTF_Font>(tfont,
                                            [](TTF_Font* mem) { if (mem) TTF_CloseFont(mem); });
        m_lastFont = &(*m_fonts.insert(std::make_pair(font, std::move(fontData))).first);
        return Font::Status::Loaded;
    }
    else
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", filename.c_str());
        return Font::Status::ErrorFileNotFound;
    }
}

void SDL2::FreeFont(const Font& font)
{
    if (m_lastFont != nullptr && m_lastFont->first == font)
        m_lastFont = nullptr;

    m_fonts.erase(font); // calls SDL2FontData destructor
}

bool SDL2::EnsureFont(const Font& font)
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

Texture::Status SDL2::LoadTexture(const Texture& texture)
{
    FreeTexture(texture);
    m_lastTexture = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

    SDL_Texture *tex = nullptr;
    SDL2TextureData texData;
    if (texture.readable)
    {
        // You cannot find the format of a texture once loaded to read from it
        // in SDL2 so we have to keep the surface to read from.
        SDL_Surface *surf = IMG_Load(filename.c_str());

        if (!surf)
        {
            Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        }
        else
        {
            tex = SDL_CreateTextureFromSurface(m_renderer, surf);
            texData.surface = deleted_unique_ptr<SDL_Surface>(surf,
                                        [](SDL_Surface* mem) { if (mem) SDL_FreeSurface(mem); });
            texData.readable = true;
        }
    }
    else
    {
        // Don't need to read. Just load straight into render format.
        tex = IMG_LoadTexture(m_renderer, filename.c_str());
        if (!tex)
        {
            Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        }
        else
        {
            texData.readable = false;
        }
    }

    if (tex)
    {
        int w, h;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        texData.texture = deleted_unique_ptr<SDL_Texture>(tex, [](SDL_Texture* mem) { if (mem) SDL_DestroyTexture(mem); });
        texData.width = w;
        texData.height = h;
        m_lastTexture = &(*m_textures.insert(std::make_pair(texture, std::move(texData))).first);
        return Texture::Status::Loaded;
    }
    else
    {
        return Texture::Status::ErrorFileNotFound;
    }
}

void SDL2::FreeTexture(const Texture& texture)
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        m_lastTexture = nullptr;

    m_textures.erase(texture); // calls SDLTextureData destructor
}

TextureData SDL2::GetTextureData(const Texture& texture) const
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

bool SDL2::EnsureTexture(const Gwk::Texture& texture)
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

SDL2::SDL2(ResourcePaths& paths, SDL_Window *window)
    :   Base(paths)
    ,   m_lastFont(nullptr)
    ,   m_lastTexture(nullptr)
    ,   m_window(window)
    ,   m_renderer(nullptr)
{
  m_renderer = SDL_GetRenderer( m_window );
}

SDL2::~SDL2()
{
}

void SDL2::SetDrawColor(Gwk::Color color)
{
    m_color.r = color.r;
    m_color.g = color.g;
    m_color.b = color.b;
    m_color.a = color.a;

    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void SDL2::RenderText(const Gwk::Font& font, Gwk::Point pos, const Gwk::String& text)
{
    if (!EnsureFont(font))
        return;

    SDL2FontData& fontData = m_lastFont->second;

    Translate(pos.x, pos.y);

    SDL_Surface *surf = TTF_RenderUTF8_Blended(fontData.tFont.get(), text.c_str(), m_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    SDL_FreeSurface(surf);

    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    const SDL_Rect dest = { pos.x,pos.y, w,h };

    SDL_RenderCopy(m_renderer, texture, nullptr, &dest);

    SDL_DestroyTexture(texture);
}

Gwk::Point SDL2::MeasureText(const Gwk::Font& font, const Gwk::String& text)
{
    if (!EnsureFont(font))
        return Gwk::Point(0, 0);

    SDL2FontData& fontData = m_lastFont->second;
    
    int w,h;
    TTF_SizeUTF8(fontData.tFont.get(), text.c_str(), &w,&h);

    return Point(w,h);
}

void SDL2::StartClip()
{
    const Gwk::Rect &rect = ClipRegion();

    // SDL2 appears to have a bug pre-version 2.0.5 where the clipping rectangle
    // is inverted vertically. See: https://github.com/billyquith/GWork/issues/18
    // Deal with backwards compatibility for now.
#if SDL_VERSION_ATLEAST(2, 0, 5)
    const SDL_Rect clip = { rect.x, rect.y, rect.w, rect.h };
#else
    int w, h;
    SDL_GetRendererOutputSize(SDL_GetRenderer(m_window), &w, &h);

    // Something wrong with clip rectangles in SDL2?
    // https://bugzilla.libsdl.org/show_bug.cgi?id=2700
    // =>   clip.y = screenHeight - clip.y - clip.h;
    const SDL_Rect clip = { rect.x, h - rect.y - rect.h, rect.w, rect.h };
#endif

    SDL_RenderSetClipRect(m_renderer, &clip);
}

void SDL2::EndClip()
{
    SDL_RenderSetClipRect(m_renderer, 0);
}

void SDL2::DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect rect,
                            float u1, float v1, float u2, float v2)
{
    if (!EnsureTexture(texture))
        return DrawMissingImage(rect);

    Translate(rect);

    SDL2TextureData& texData = m_lastTexture->second;

    const unsigned int w = texData.width;
    const unsigned int h = texData.height;

    const SDL_Rect source = { int(u1*w), int(v1*h), int((u2-u1)*w), int((v2-v1)*h) },
                     dest = { rect.x, rect.y, rect.w, rect.h };

    SDL_RenderCopy(m_renderer, texData.texture.get(), &source, &dest);
}

Gwk::Color SDL2::PixelColor(const Gwk::Texture& texture, unsigned int x, unsigned int y,
                              const Gwk::Color& col_default)
{
    if (!EnsureTexture(texture))
        return col_default;

    SDL2TextureData& texData = m_lastTexture->second;

    if (!texData.readable || !texData.surface)
        return col_default;

    if (SDL_MUSTLOCK(texData.surface.get()) != 0)
        SDL_LockSurface(texData.surface.get());

    const char *mem = static_cast<char*>(texData.surface->pixels) + y* texData.surface->pitch + x*sizeof(Uint32);
    const Uint32 pix = *reinterpret_cast<const Uint32*>(mem);

    Gwk::Color col;
    SDL_GetRGBA(pix, texData.surface->format, &col.r, &col.g, &col.b, &col.a);

    if (SDL_MUSTLOCK(texData.surface.get()) != 0)
        SDL_UnlockSurface(texData.surface.get());

    return col;
}

void SDL2::DrawFilledRect(Gwk::Rect rect)
{
    Translate(rect);

    const SDL_Rect srect = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderFillRect(m_renderer, &srect);
}

void SDL2::DrawLinedRect(Gwk::Rect rect)
{
    Translate(rect);

    const SDL_Rect srect = { rect.x, rect.y, rect.w, rect.h };
    SDL_RenderDrawRect(m_renderer, &srect);
}

bool SDL2::BeginContext(Gwk::WindowProvider* )
{
    SDL_RenderClear(m_renderer);
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    return true;
}

bool SDL2::EndContext(Gwk::WindowProvider* window)
{
    return true;
}

bool SDL2::PresentContext(Gwk::WindowProvider* window)
{
    SDL_RenderPresent(m_renderer);
    return true;
}

} // Renderer
} // Gwork
