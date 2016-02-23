
// SDL2 Renderer for GWEN - http://www.libsdl.org/
// Added by BQ.

#include <Gwen/Gwen.h>
#include <Gwen/BaseRender.h>
#include <Gwen/Utility.h>
#include <Gwen/Font.h>
#include <Gwen/Texture.h>
#include <Gwen/Renderers/SDL2.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace Gwen
{
    namespace Renderer
    {

        SDL2::SDL2(SDL_Window *window)
        :   m_window(window)
        ,   m_renderer(NULL)
        {
            m_renderer = SDL_CreateRenderer(m_window, -1,
                                            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        }

        SDL2::~SDL2()
        {
            SDL_DestroyRenderer(m_renderer);
        }

        void SDL2::SetDrawColor(Gwen::Color color)
        {
            m_color.r = color.r;
            m_color.g = color.g;
            m_color.b = color.b;
            m_color.a = color.a;
            
            SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
        }

        void SDL2::LoadFont(Gwen::Font* font)
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

        void SDL2::FreeFont(Gwen::Font* pFont)
        {
            if (pFont->data)
            {
                TTF_CloseFont(static_cast<TTF_Font*>(pFont->data));
                pFont->data = NULL;
            }
        }

        void SDL2::RenderText(Gwen::Font* pFont, Gwen::Point pos, const Gwen::String& text)
        {
            TTF_Font *tfont = static_cast<TTF_Font*>(pFont->data);
            Translate(pos.x, pos.y);
            
            SDL_Surface *surf = TTF_RenderUTF8_Blended(tfont, text.c_str(), m_color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(m_renderer, surf);
            SDL_FreeSurface(surf);
            
            int w, h;
            SDL_QueryTexture(texture, NULL, NULL, &w, &h);
            const SDL_Rect dest = { pos.x,pos.y, w,h };
            
            SDL_RenderCopy(m_renderer, texture, NULL, &dest);
            
            SDL_DestroyTexture(texture);
        }

        Gwen::Point SDL2::MeasureText(Gwen::Font* pFont, const Gwen::String& text)
        {
            TTF_Font *tfont = static_cast<TTF_Font*>(pFont->data);

            // If the font doesn't exist, or the font size should be changed.
            if (!tfont || pFont->realsize != pFont->size*Scale())
            {
                FreeFont(pFont);
                LoadFont(pFont);
                tfont = static_cast<TTF_Font*>(pFont->data);
            }

            if (!tfont)
                return Gwen::Point(0, 0);

            int w,h;
            TTF_SizeUTF8(tfont, text.c_str(), &w,&h);
            
            return Point(w,h);
        }

        void SDL2::StartClip()
        {
            const Gwen::Rect &rect = ClipRegion();
            const SDL_Rect clip = { rect.x,rect.y, rect.w,rect.h };
            SDL_RenderSetClipRect(m_renderer, &clip);
        }

        void SDL2::EndClip()
        {
            SDL_RenderSetClipRect(m_renderer, NULL);
        }

        void SDL2::LoadTexture(Gwen::Texture* pTexture)
        {
            if (!pTexture)
                return;

            if (pTexture->data)
                FreeTexture(pTexture);
            
            SDL_Texture *tex = NULL;
            if (pTexture->readable)
            {
                // You cannot find the format of a texture once loaded to read from it
                // in SDL2 so we have to keep the surface to read from.
                SDL_Surface *surf = IMG_Load(pTexture->name.c_str());
                tex = SDL_CreateTextureFromSurface(m_renderer, surf);
                pTexture->surface = surf;
            }
            else
            {
                // Don't need to read. Just load straight into render format.
                tex = IMG_LoadTexture(m_renderer, pTexture->name.c_str());
            }

            if (tex)
            {
                int w, h;
                SDL_QueryTexture(tex, NULL, NULL, &w, &h);
                
                pTexture->data = tex;
                pTexture->width = w;
                pTexture->height = h;
                pTexture->failed = false;
            }
            else
            {
                pTexture->data = NULL;
                pTexture->failed = true;
            }
        }

        void SDL2::FreeTexture(Gwen::Texture* pTexture)
        {
            SDL_DestroyTexture(static_cast<SDL_Texture*>(pTexture->data));
            pTexture->data = NULL;
            
            if (pTexture->surface)
            {
                SDL_FreeSurface(static_cast<SDL_Surface*>(pTexture->surface));
                pTexture->surface = NULL;
                pTexture->readable = false;
            }
        }

        void SDL2::DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect rect,
                                    float u1, float v1, float u2, float v2)
        {
            SDL_Texture *tex = static_cast<SDL_Texture*>(pTexture->data);

            if (!tex)
                return DrawMissingImage(rect);

            Translate(rect);
            
            const unsigned int w = pTexture->width;
            const unsigned int h = pTexture->height;
            
            const SDL_Rect source = { int(u1*w), int(v1*h), int((u2-u1)*w), int((v2-v1)*h) },
                             dest = { rect.x, rect.y, rect.w, rect.h };

            SDL_RenderCopy(m_renderer, tex, &source, &dest);
        }

        Gwen::Color SDL2::PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y,
                                      const Gwen::Color& col_default)
        {
            SDL_Surface *surf = static_cast<SDL_Surface*>(pTexture->surface);

            if (!pTexture->readable || !surf)
                return col_default;
            
            if (SDL_MUSTLOCK(surf) != 0)
                SDL_LockSurface(surf);

            const char *mem = static_cast<char*>(surf->pixels) + y*surf->pitch + x*sizeof(Uint32);
            const Uint32 pix = *reinterpret_cast<const Uint32*>(mem);

            Gwen::Color col;
            SDL_GetRGBA(pix, surf->format, &col.r, &col.g, &col.b, &col.a);

            if (SDL_MUSTLOCK(surf) != 0)
                SDL_UnlockSurface(surf);

            return col;
        }

        void SDL2::DrawFilledRect(Gwen::Rect rect)
        {
            Translate(rect);
            
            const SDL_Rect srect = { rect.x, rect.y, rect.w, rect.h };
            SDL_RenderFillRect(m_renderer, &srect);
        }

        void SDL2::DrawLinedRect(Gwen::Rect rect)
        {
            Translate(rect);
            
            const SDL_Rect srect = { rect.x, rect.y, rect.w, rect.h };
            SDL_RenderDrawRect(m_renderer, &srect);
        }

        bool SDL2::BeginContext(Gwen::WindowProvider* )
        {
            SDL_RenderClear(m_renderer);
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
            return true;
        }

        bool SDL2::EndContext(Gwen::WindowProvider* pWindow)
        {
            return true;
        }

        bool SDL2::PresentContext(Gwen::WindowProvider* pWindow)
        {
            SDL_RenderPresent(m_renderer);
            return true;
        }

        
    } // Renderer
} // Gwen
