
// SDL2 Renderer for GWEN - http://www.libsdl.org/
// Added by BQ.

#include <Gwen/Gwen.h>
#include <Gwen/BaseRender.h>
#include <Gwen/Utility.h>
#include <Gwen/Font.h>
#include <Gwen/Texture.h>
#include <Gwen/Renderers/SDL2.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

namespace Gwen
{
    namespace Renderer
    {
        //
        // SDL2 texture cache.
        //
//        class AllegroCTT : public ICacheToTexture
//        {
//        public:
//            
//            AllegroCTT() : m_oldTarget(NULL) {}
//            ~AllegroCTT() {}
//            
//            void Initialize() {}
//            void ShutDown();
//            void SetRenderer(Gwen::Renderer::Base* renderer) { m_renderer = renderer; }
//            
//            void SetupCacheTexture(Gwen::Controls::Base* control);
//            void FinishCacheTexture(Gwen::Controls::Base* control);
//            
//            void DrawCachedControlTexture(Gwen::Controls::Base* control);
//            void CreateControlCacheTexture(Gwen::Controls::Base* control);
//            void UpdateControlCacheTexture(Gwen::Controls::Base* control) {}
//            
//            // TODO What destroys the cached textures? Does this assume they always exist?
//            
//        private:
//            
//            Gwen::Renderer::Base *m_renderer;
//            
//            struct CacheEntry
//            {
//                ALLEGRO_BITMAP *m_bitmap;
//            };
//            
//            typedef Gwen::Controls::Base* Key;
//            typedef std::map< Key, CacheEntry > CacheMap;
//            CacheMap m_cache;
//            
//            ALLEGRO_BITMAP *m_oldTarget;
//        };
//
//        void AllegroCTT::ShutDown()
//        {
//            // TODO - Delete the cached textures we created.
//            // Note: This doesn't get called at the moment because Gwen currently crashes
//            //       if we delete the renderer on clean up.
//        }
//        
//        void AllegroCTT::CreateControlCacheTexture(Gwen::Controls::Base* control)
//        {
//            // If we haven't seen this control before, create a new entry.
//            if (m_cache.find(control) == m_cache.end())
//            {
//                const Gwen::Rect &bounds = control->GetBounds();
//                const int w = bounds.w, h = bounds.h;
//                
//                CacheEntry newEntry = { al_create_bitmap(w, h) };
//                m_cache.insert(std::pair<Key,CacheEntry>(control, newEntry));
//            }
//        }
//
//        void AllegroCTT::SetupCacheTexture(Gwen::Controls::Base* control)
//        {
//            CacheMap::iterator it = m_cache.find(control);
//            assert(it != m_cache.end());
//            if (it != m_cache.end())
//            {
//                // Prepare for rendering.
//                assert(m_oldTarget==NULL);
//                m_oldTarget = al_get_target_bitmap();
//                al_set_target_bitmap((*it).second.m_bitmap);
//                al_clear_to_color(al_map_rgb_f(1.f,1.f,1.f));
//            }
//        }
//
//        void AllegroCTT::FinishCacheTexture(Gwen::Controls::Base* control)
//        {
//            // Prepare for rendering.
//            al_set_target_bitmap(m_oldTarget);
//            m_oldTarget = NULL;
//        }
//        
//        void AllegroCTT::DrawCachedControlTexture(Gwen::Controls::Base* control)
//        {
//            CacheMap::iterator it = m_cache.find(control);
//            assert(it != m_cache.end());
//            if (it != m_cache.end())
//            {
//                ALLEGRO_BITMAP *bmp = (*it).second.m_bitmap;                
//                const Gwen::Point &pos = m_renderer->GetRenderOffset();
//                al_draw_bitmap(bmp, pos.x, pos.y, 0);
//            }
//        }

        //-------------------------------------------------------------------------------

        SDL2::SDL2(SDL_Window *window)
        :   m_window(window)
        ,   m_renderer(NULL)
//        :   m_ctt(new AllegroCTT)
        {
//            m_ctt->SetRenderer(this);
//            m_ctt->Initialize();
            
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

        void SDL2::RenderText(Gwen::Font* pFont, Gwen::Point pos,
                                 const Gwen::String& text)
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
        }

        Gwen::Point SDL2::MeasureText(Gwen::Font* pFont, const Gwen::String& text)
        {
            TTF_Font *tfont = static_cast<TTF_Font*>(pFont->data);

            // If the font doesn't exist, or the font size should be changed
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

            SDL_Texture *bmp = IMG_LoadTexture(m_renderer, pTexture->name.c_str());
            
            if (bmp)
            {
                int w, h;
                SDL_QueryTexture(bmp, NULL, NULL, &w, &h);
                
                pTexture->data = bmp;
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
        }

        void SDL2::DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect rect,
                                       float u1, float v1,
                                       float u2, float v2)
        {
            SDL_Texture *bmp = static_cast<SDL_Texture*>(pTexture->data);

            if (!bmp)
                return DrawMissingImage(rect);

            Translate(rect);
            
            const unsigned int w = pTexture->width;
            const unsigned int h = pTexture->height;
            
            const SDL_Rect source = { int(u1*w), int(v1*h), int((u2-u1)*w), int((v2-v1)*h) },
                             dest = { rect.x, rect.y, rect.w, rect.h };

            SDL_RenderCopy(m_renderer, bmp, &source, &dest);
        }

        Gwen::Color SDL2::PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y,
                                         const Gwen::Color& col_default)
        {
//            ALLEGRO_BITMAP* bmp = (ALLEGRO_BITMAP*)pTexture->data;
//
//            if (!bmp)
//                return col_default;
//
//            ALLEGRO_COLOR col = al_get_pixel(bmp, x, y);
//            Gwen::Color gcol;
//            al_unmap_rgba(col, &gcol.r, &gcol.g, &gcol.b, &gcol.a);
//            return gcol;
            return Color(128,128,128,255);
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

//        void SDL2::DrawShavedCornerRect(Gwen::Rect rect, bool bSlight)
//        {
//            // Draw INSIDE the w/h.
//            rect.w -= 1;
//            rect.h -= 1;
//            
//#define SET_VERT(I, X,Y)            vtx[I].x = (X), vtx[I].y = (Y), vtx[I].color = m_Color
//#define ADD_LINE(I, X0,Y0, X1,Y1)   SET_VERT(I, X0,Y0); SET_VERT(I+1, X1,Y1)
//
//            const float fx = rect.x+0.5f, fy = rect.y+0.5f;
//            const float fw = rect.w, fh = rect.h;
//
//            if (bSlight)
//            {
//                //    DrawFilledRect(Gwen::Rect(rect.x+1, rect.y, rect.w-1, 1));
//                //    DrawFilledRect(Gwen::Rect(rect.x+1, rect.y+rect.h, rect.w-1, 1));
//                //    DrawFilledRect(Gwen::Rect(rect.x, rect.y+1, 1, rect.h-1));
//                //    DrawFilledRect(Gwen::Rect(rect.x+rect.w, rect.y+1, 1, rect.h-1));
//                
//                ALLEGRO_VERTEX vtx[4*2];
//                ADD_LINE(0, fx+1.f,fy,         fx+fw-1.f,fy   ); // top
//                ADD_LINE(2, fx+fw,fy+1.f,      fx+fw,fy+fh-1.f); // right
//                ADD_LINE(4, fx+fw-1.f,fy+fh,   fx+1.f,fy+fh   ); // bottom
//                ADD_LINE(6, fx,fy+fh-1.f,      fx,fy+1.f      ); // left
//                al_draw_prim(vtx, NULL, NULL, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
//            }
//            else
//            {
//                //    DrawPixel(rect.x+1, rect.y+1);
//                //    DrawPixel(rect.x+rect.w-1, rect.y+1);
//                //    DrawPixel(rect.x+1, rect.y+rect.h-1);
//                //    DrawPixel(rect.x+rect.w-1, rect.y+rect.h-1);
//                //    DrawFilledRect(Gwen::Rect(rect.x+2, rect.y, rect.w-3, 1));
//                //    DrawFilledRect(Gwen::Rect(rect.x+2, rect.y+rect.h, rect.w-3, 1));
//                //    DrawFilledRect(Gwen::Rect(rect.x, rect.y+2, 1, rect.h-3));
//                //    DrawFilledRect(Gwen::Rect(rect.x+rect.w, rect.y+2, 1, rect.h-3));
//                
//                ALLEGRO_VERTEX vtx[4*2];
//                ADD_LINE(0, fx+2.f,fy,          fx+fw-2.f,fy    ); // top
//                ADD_LINE(2, fx+fw,fy+2.f,       fx+fw,fy+fh-2.f ); // right
//                ADD_LINE(4, fx+fw-2.f,fy+fh,    fx+2.f,fy+fh    ); // bottom
//                ADD_LINE(6, fx,fy+fh-2.f,       fx,fy+2.f       ); // left
//                al_draw_prim(vtx, NULL, NULL, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
//            }
//            
//#undef SET_VERT
//#undef ADD_LINE
//        }

        //    void SDL2::DrawPixel(int x, int y)
        //    {
        //        al_put_pixel(x+0.5f, y+0.5f, m_Color);
        //    }

        bool SDL2::BeginContext(Gwen::WindowProvider* )
        {
            SDL_RenderClear(m_renderer);
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
        
//        ICacheToTexture* SDL2::GetCTT()
//        {
//            return m_ctt;
//        }

        
    } // Renderer
} // Gwen
