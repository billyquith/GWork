
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

SDL2::SDL2(SDL_Window *window)
:   m_window(window)
,   m_renderer(nullptr)
{
    m_renderer = SDL_CreateRenderer(m_window, -1,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL2::~SDL2()
{
    SDL_DestroyRenderer(m_renderer);
}

void SDL2::SetDrawColor(Gwk::Color color)
{
    m_color.r = color.r;
    m_color.g = color.g;
    m_color.b = color.b;
    m_color.a = color.a;
    
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
}

void SDL2::LoadFont(Gwk::Font* font)
{
    font->realsize = font->size*Scale();
    std::string fontFile(font->facename);

    if (fontFile.find(".ttf") == std::string::npos)
        fontFile += ".ttf";

    TTF_Font *tfont = TTF_OpenFont(fontFile.c_str(), font->realsize);
    if (!tfont)
    {
        printf("Font load error: %s\n", TTF_GetError());
    }            
    
    font->data = tfont;
}

void SDL2::FreeFont(Gwk::Font* font)
{
    if (font->data)
    {
        TTF_CloseFont(static_cast<TTF_Font*>(font->data));
        font->data = nullptr;
    }
}

void SDL2::RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text)
{
    TTF_Font *tfont = static_cast<TTF_Font*>(font->data);
    Translate(pos.x, pos.y);
    
    SDL_Surface *surf = TTF_RenderUTF8_Blended(tfont, text.c_str(), m_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surf);
    SDL_FreeSurface(surf);
    
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    const SDL_Rect dest = { pos.x,pos.y, w,h };
    
    SDL_RenderCopy(m_renderer, texture, nullptr, &dest);
    
    SDL_DestroyTexture(texture);
}

Gwk::Point SDL2::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    TTF_Font *tfont = static_cast<TTF_Font*>(font->data);

    // If the font doesn't exist, or the font size should be changed.
    if (!tfont || font->realsize != font->size*Scale())
    {
        FreeFont(font);
        LoadFont(font);
        tfont = static_cast<TTF_Font*>(font->data);
    }

    if (!tfont)
        return Gwk::Point(0, 0);

    int w,h;
    TTF_SizeUTF8(tfont, text.c_str(), &w,&h);
    
    return Point(w,h);
}

void SDL2::StartClip()
{
    const Gwk::Rect &rect = ClipRegion();
    const SDL_Rect clip = { rect.x, rect.y, rect.w,rect.h };
    
//          TODO - BAD! There seems to be something wrong with clip rectangles in SDL2.
//          https://bugzilla.libsdl.org/show_bug.cgi?id=2700
//            SDL_RenderSetClipRect(m_renderer, &clip);
}

void SDL2::EndClip()
{
    SDL_RenderSetClipRect(m_renderer, 0);
}

void SDL2::LoadTexture(Gwk::Texture* texture)
{
    if (!texture)
        return;

    if (texture->data)
        FreeTexture(texture);
    
    SDL_Texture *tex = nullptr;
    if (texture->readable)
    {
        // You cannot find the format of a texture once loaded to read from it
        // in SDL2 so we have to keep the surface to read from.
        SDL_Surface *surf = IMG_Load(texture->name.c_str());
        tex = SDL_CreateTextureFromSurface(m_renderer, surf);
        texture->surface = surf;
    }
    else
    {
        // Don't need to read. Just load straight into render format.
        tex = IMG_LoadTexture(m_renderer, texture->name.c_str());
    }

    if (tex)
    {
        int w, h;
        SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
        
        texture->data = tex;
        texture->width = w;
        texture->height = h;
        texture->failed = false;
    }
    else
    {
        texture->data = nullptr;
        texture->failed = true;
    }
}

void SDL2::FreeTexture(Gwk::Texture* texture)
{
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture->data));
    texture->data = nullptr;
    
    if (texture->surface)
    {
        SDL_FreeSurface(static_cast<SDL_Surface*>(texture->surface));
        texture->surface = nullptr;
        texture->readable = false;
    }
}

void SDL2::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                            float u1, float v1, float u2, float v2)
{
    SDL_Texture *tex = static_cast<SDL_Texture*>(texture->data);

    if (!tex)
        return DrawMissingImage(rect);

    Translate(rect);
    
    const unsigned int w = texture->width;
    const unsigned int h = texture->height;
    
    const SDL_Rect source = { int(u1*w), int(v1*h), int((u2-u1)*w), int((v2-v1)*h) },
                     dest = { rect.x, rect.y, rect.w, rect.h };

    SDL_RenderCopy(m_renderer, tex, &source, &dest);
}

Gwk::Color SDL2::PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                              const Gwk::Color& col_default)
{
    SDL_Surface *surf = static_cast<SDL_Surface*>(texture->surface);

    if (!texture->readable || !surf)
        return col_default;
    
    if (SDL_MUSTLOCK(surf) != 0)
        SDL_LockSurface(surf);

    const char *mem = static_cast<char*>(surf->pixels) + y*surf->pitch + x*sizeof(Uint32);
    const Uint32 pix = *reinterpret_cast<const Uint32*>(mem);

    Gwk::Color col;
    SDL_GetRGBA(pix, surf->format, &col.r, &col.g, &col.b, &col.a);

    if (SDL_MUSTLOCK(surf) != 0)
        SDL_UnlockSurface(surf);

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
