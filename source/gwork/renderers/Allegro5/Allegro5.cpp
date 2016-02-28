#include "Gwork/Gwork.h"
#include "Gwork/BaseRender.h"
#include "Gwork/Utility.h"
#include "Gwork/Font.h"
#include "Gwork/Texture.h"
#include "Gwork/Renderers/Allegro.h"

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

namespace Gwk
{
    namespace Renderer
    {
        //
        // Allegro texture cache.
        //
        class AllegroCTT : public ICacheToTexture
        {
        public:
            
            AllegroCTT() : m_oldTarget(NULL) {}
            ~AllegroCTT() {}
            
            void Initialize() {}
            void ShutDown();
            void SetRenderer(Gwk::Renderer::Base* renderer) { m_renderer = renderer; }
            
            void SetupCacheTexture(Gwk::Controls::Base* control);
            void FinishCacheTexture(Gwk::Controls::Base* control);
            
            void DrawCachedControlTexture(Gwk::Controls::Base* control);
            void CreateControlCacheTexture(Gwk::Controls::Base* control);
            void UpdateControlCacheTexture(Gwk::Controls::Base* control) {}
            
            // TODO What destroys the cached textures? Does this assume they always exist?
            
        private:
            
            Gwk::Renderer::Base *m_renderer;
            
            struct CacheEntry
            {
                ALLEGRO_BITMAP *m_bitmap;
            };
            
            typedef Gwk::Controls::Base* Key;
            typedef std::map< Key, CacheEntry > CacheMap;
            CacheMap m_cache;
            
            ALLEGRO_BITMAP *m_oldTarget;
        };

        void AllegroCTT::ShutDown()
        {
            // TODO - Delete the cached textures we created.
            // Note: This doesn't get called at the moment because Gwork currently crashes
            //       if we delete the renderer on clean up.
        }
        
        void AllegroCTT::CreateControlCacheTexture(Gwk::Controls::Base* control)
        {
            // If we haven't seen this control before, create a new entry.
            if (m_cache.find(control) == m_cache.end())
            {
                const Gwk::Rect &bounds = control->GetBounds();
                const int w = bounds.w, h = bounds.h;
                
                CacheEntry newEntry = { al_create_bitmap(w, h) };
                m_cache.insert(std::pair<Key,CacheEntry>(control, newEntry));
            }
        }

        void AllegroCTT::SetupCacheTexture(Gwk::Controls::Base* control)
        {
            CacheMap::iterator it = m_cache.find(control);
            assert(it != m_cache.end());
            if (it != m_cache.end())
            {
                // Prepare for rendering.
                assert(m_oldTarget==NULL);
                m_oldTarget = al_get_target_bitmap();
                al_set_target_bitmap((*it).second.m_bitmap);
                al_clear_to_color(al_map_rgb_f(1.f,1.f,1.f));
            }
        }

        void AllegroCTT::FinishCacheTexture(Gwk::Controls::Base* control)
        {
            // Prepare for rendering.
            al_set_target_bitmap(m_oldTarget);
            m_oldTarget = NULL;
        }
        
        void AllegroCTT::DrawCachedControlTexture(Gwk::Controls::Base* control)
        {
            CacheMap::iterator it = m_cache.find(control);
            assert(it != m_cache.end());
            if (it != m_cache.end())
            {
                ALLEGRO_BITMAP *bmp = (*it).second.m_bitmap;                
                const Gwk::Point &pos = m_renderer->GetRenderOffset();
                al_draw_bitmap(bmp, pos.x, pos.y, 0);
            }
        }

        //-------------------------------------------------------------------------------

        Allegro::Allegro()
        :   m_ctt(new AllegroCTT)
        {
            m_ctt->SetRenderer(this);
            m_ctt->Initialize();
        }

        Allegro::~Allegro()
        {
        }

        void Allegro::SetDrawColor(Gwk::Color color)
        {
            m_Color = al_map_rgba(color.r, color.g, color.b, color.a);
        }

        void Allegro::LoadFont(Gwk::Font* font)
        {
            font->realsize = font->size*Scale();
            std::string fontName(font->facename);

            if (fontName.find(".ttf") == std::string::npos)
                fontName += ".ttf";

            ALLEGRO_FONT* afont = al_load_font(fontName.c_str(),
                                               font->realsize,
                                               ALLEGRO_TTF_NO_KERNING);
            font->data = afont;
        }

        void Allegro::FreeFont(Gwk::Font* pFont)
        {
            if (pFont->data)
            {
                al_destroy_font((ALLEGRO_FONT*)pFont->data);
                pFont->data = NULL;
            }
        }

        void Allegro::RenderText(Gwk::Font* pFont, Gwk::Point pos,
                                 const Gwk::String& text)
        {
            ALLEGRO_FONT *afont = (ALLEGRO_FONT*)pFont->data;
            Translate(pos.x, pos.y);
            al_draw_text(afont, m_Color, pos.x, pos.y, ALLEGRO_ALIGN_LEFT, text.c_str());
        }

        Gwk::Point Allegro::MeasureText(Gwk::Font* pFont, const Gwk::String& text)
        {
            ALLEGRO_FONT* afont = (ALLEGRO_FONT*)pFont->data;

            // If the font doesn't exist, or the font size should be changed
            if (!afont || pFont->realsize != pFont->size*Scale())
            {
                FreeFont(pFont);
                LoadFont(pFont);
                afont = (ALLEGRO_FONT*)pFont->data;
            }

            if (!afont)
                return Gwk::Point(0, 0);

            return Point(al_get_text_width(afont, text.c_str()), al_get_font_line_height(afont));
        }

        void Allegro::StartClip()
        {
            Gwk::Rect rect = ClipRegion();
            al_set_clipping_rectangle(rect.x, rect.y, rect.w, rect.h);
        }

        void Allegro::EndClip()
        {
            ALLEGRO_BITMAP* targ = al_get_target_bitmap();
            al_set_clipping_rectangle(0, 0,
                                      al_get_bitmap_width(targ), al_get_bitmap_height(targ));
        }

        void Allegro::LoadTexture(Gwk::Texture* pTexture)
        {
            if (!pTexture)
                return;

            if (pTexture->data)
                FreeTexture(pTexture);

            ALLEGRO_BITMAP* bmp = al_load_bitmap(pTexture->name.c_str());

            if (bmp)
            {
                pTexture->data = bmp;
                pTexture->width = al_get_bitmap_width(bmp);
                pTexture->height = al_get_bitmap_height(bmp);
                pTexture->failed = false;
            }
            else
            {
                pTexture->data = NULL;
                pTexture->failed = true;
            }
        }

        void Allegro::FreeTexture(Gwk::Texture* pTexture)
        {
            al_destroy_bitmap((ALLEGRO_BITMAP*)pTexture->data);
            pTexture->data = NULL;
        }

        void Allegro::DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect rect,
                                       float u1, float v1,
                                       float u2, float v2)
        {
            ALLEGRO_BITMAP* bmp = (ALLEGRO_BITMAP*)pTexture->data;

            if (!bmp)
                return DrawMissingImage(rect);

            Translate(rect);
            const unsigned int w = pTexture->width;
            const unsigned int h = pTexture->height;
            al_draw_scaled_bitmap(bmp,
                                  u1*w, v1*h, (u2-u1)*w, (v2-v1)*h,  // source
                                  rect.x, rect.y, rect.w, rect.h,    // destination
                                  0);
        }

        Gwk::Color Allegro::PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y,
                                         const Gwk::Color& col_default)
        {
            ALLEGRO_BITMAP* bmp = (ALLEGRO_BITMAP*)pTexture->data;

            if (!bmp)
                return col_default;

            ALLEGRO_COLOR col = al_get_pixel(bmp, x, y);
            Gwk::Color gcol;
            al_unmap_rgba(col, &gcol.r, &gcol.g, &gcol.b, &gcol.a);
            return gcol;
        }

        void Allegro::DrawFilledRect(Gwk::Rect rect)
        {
            Translate(rect);
            const float fx = rect.x+0.5f, fy = rect.y+0.5f;
            al_draw_filled_rectangle(fx, fy, fx+rect.w, fy+rect.h, m_Color);
        }

        void Allegro::DrawLinedRect(Gwk::Rect rect)
        {
            Translate(rect);
            // Width of 0 draws a line, not a rect of width 1.
            const float fx = rect.x+0.5f, fy = rect.y+0.5f;
            al_draw_rectangle(fx, fy, fx+rect.w, fy+rect.h, m_Color, 0.f);
        }

        void Allegro::DrawShavedCornerRect(Gwk::Rect rect, bool bSlight)
        {
            // Draw INSIDE the w/h.
            rect.w -= 1;
            rect.h -= 1;
            
#define SET_VERT(I, X,Y)            vtx[I].x = (X), vtx[I].y = (Y), vtx[I].color = m_Color
#define ADD_LINE(I, X0,Y0, X1,Y1)   SET_VERT(I, X0,Y0); SET_VERT(I+1, X1,Y1)

            const float fx = rect.x+0.5f, fy = rect.y+0.5f;
            const float fw = rect.w, fh = rect.h;

            if (bSlight)
            {
                //    DrawFilledRect(Gwk::Rect(rect.x+1, rect.y, rect.w-1, 1));
                //    DrawFilledRect(Gwk::Rect(rect.x+1, rect.y+rect.h, rect.w-1, 1));
                //    DrawFilledRect(Gwk::Rect(rect.x, rect.y+1, 1, rect.h-1));
                //    DrawFilledRect(Gwk::Rect(rect.x+rect.w, rect.y+1, 1, rect.h-1));
                
                ALLEGRO_VERTEX vtx[4*2];
                ADD_LINE(0, fx+1.f,fy,         fx+fw-1.f,fy   ); // top
                ADD_LINE(2, fx+fw,fy+1.f,      fx+fw,fy+fh-1.f); // right
                ADD_LINE(4, fx+fw-1.f,fy+fh,   fx+1.f,fy+fh   ); // bottom
                ADD_LINE(6, fx,fy+fh-1.f,      fx,fy+1.f      ); // left
                al_draw_prim(vtx, NULL, NULL, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
            }
            else
            {
                //    DrawPixel(rect.x+1, rect.y+1);
                //    DrawPixel(rect.x+rect.w-1, rect.y+1);
                //    DrawPixel(rect.x+1, rect.y+rect.h-1);
                //    DrawPixel(rect.x+rect.w-1, rect.y+rect.h-1);
                //    DrawFilledRect(Gwk::Rect(rect.x+2, rect.y, rect.w-3, 1));
                //    DrawFilledRect(Gwk::Rect(rect.x+2, rect.y+rect.h, rect.w-3, 1));
                //    DrawFilledRect(Gwk::Rect(rect.x, rect.y+2, 1, rect.h-3));
                //    DrawFilledRect(Gwk::Rect(rect.x+rect.w, rect.y+2, 1, rect.h-3));
                
                ALLEGRO_VERTEX vtx[4*2];
                ADD_LINE(0, fx+2.f,fy,          fx+fw-2.f,fy    ); // top
                ADD_LINE(2, fx+fw,fy+2.f,       fx+fw,fy+fh-2.f ); // right
                ADD_LINE(4, fx+fw-2.f,fy+fh,    fx+2.f,fy+fh    ); // bottom
                ADD_LINE(6, fx,fy+fh-2.f,       fx,fy+2.f       ); // left
                al_draw_prim(vtx, NULL, NULL, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
            }
            
#undef SET_VERT
#undef ADD_LINE
        }

        //  Unfortunately I think we need to lock the target texture to use this. The default
        //  is that a rect of size (1,1) will be drawn.
        //
        //    void Allegro::DrawPixel(int x, int y)
        //    {
        //        al_put_pixel(x+0.5f, y+0.5f, m_Color);
        //    }

        bool Allegro::BeginContext(Gwk::WindowProvider* pWindow)
        {
            al_clear_to_color(al_map_rgba_f(0.f, 0.f, 0.f, 0.f));
            return true;
        }

        bool Allegro::EndContext(Gwk::WindowProvider* pWindow)
        {
            return true;
        }

        bool Allegro::PresentContext(Gwk::WindowProvider* pWindow)
        {
            al_flip_display();
            return true;
        }
        
        ICacheToTexture* Allegro::GetCTT()
        {
            return m_ctt;
        }

        
    } // Renderer
} // Gwork
