/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/BaseRender.h>
#include <Gwork/Renderers/Allegro5.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#include <map>

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
    
    AllegroCTT() : m_oldTarget(nullptr) {}
    ~AllegroCTT() {}
    
    void Initialize() {}
    void ShutDown();
    void SetRenderer(Gwk::Renderer::Base* renderer) { m_renderer = renderer; }
    
    void SetupCacheTexture(CacheHandle control);
    void FinishCacheTexture(CacheHandle control);
    
    void DrawCachedControlTexture(CacheHandle control);
    void CreateControlCacheTexture(CacheHandle control, const Point& size);
    void UpdateControlCacheTexture(CacheHandle control) {}
    
    // TODO What destroys the cached textures? Does this assume they always exist?
    
private:
    
    Gwk::Renderer::Base *m_renderer;
    
    struct CacheEntry
    {
        ALLEGRO_BITMAP *m_bitmap;
    };
    
    typedef std::map< CacheHandle, CacheEntry > CacheMap;
    CacheMap m_cache;
    
    ALLEGRO_BITMAP *m_oldTarget;
};

void AllegroCTT::ShutDown()
{
    // TODO: - Delete the cached textures we created.
    // Note: This doesn't get called at the moment because Gwork currently crashes
    //       if we delete the renderer on clean up.
}

void AllegroCTT::CreateControlCacheTexture(CacheHandle control, const Point& size)
{
    // If we haven't seen this control before, create a new entry.
    if (m_cache.find(control) == m_cache.end())
    {
        al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
        CacheEntry newEntry = { al_create_bitmap(size.x, size.y) };
        m_cache.insert(std::pair<CacheHandle,CacheEntry>(control, newEntry));
    }
}

void AllegroCTT::SetupCacheTexture(CacheHandle control)
{
    CacheMap::iterator it = m_cache.find(control);
    assert(it != m_cache.end());
    if (it != m_cache.end())
    {
        // Prepare for rendering.
        assert(m_oldTarget==nullptr);
        m_oldTarget = al_get_target_bitmap();
        
        auto albmp = it->second.m_bitmap;
        assert(albmp != nullptr);
        al_set_target_bitmap(albmp);
        
        al_clear_to_color(al_map_rgb_f(1.f, 1.f, 1.f));
    }
}

void AllegroCTT::FinishCacheTexture(CacheHandle control)
{
    // Prepare for rendering.
    al_set_target_bitmap(m_oldTarget);
    m_oldTarget = nullptr;
}

void AllegroCTT::DrawCachedControlTexture(CacheHandle control)
{
    CacheMap::iterator it = m_cache.find(control);
    assert(it != m_cache.end());
    if (it != m_cache.end())
    {
        ALLEGRO_BITMAP *bmp = it->second.m_bitmap;
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
    m_color = al_map_rgba(color.r, color.g, color.b, color.a);
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

void Allegro::FreeFont(Gwk::Font* font)
{
    if (font->data)
    {
        al_destroy_font((ALLEGRO_FONT*)font->data);
        font->data = nullptr;
    }
}

void Allegro::RenderText(Gwk::Font* font, Gwk::Point pos,
                         const Gwk::String& text)
{
    ALLEGRO_FONT *afont = (ALLEGRO_FONT*)font->data;
    Translate(pos.x, pos.y);
    al_draw_text(afont, m_color, pos.x, pos.y, ALLEGRO_ALIGN_LEFT, text.c_str());
}

Gwk::Point Allegro::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    ALLEGRO_FONT* afont = (ALLEGRO_FONT*)font->data;

    // If the font doesn't exist, or the font size should be changed
    if (!afont || font->realsize != font->size*Scale())
    {
        FreeFont(font);
        LoadFont(font);
        afont = (ALLEGRO_FONT*)font->data;
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

void Allegro::LoadTexture(Gwk::Texture* texture)
{
    if (!texture)
        return;

    if (texture->data)
        FreeTexture(texture);

    ALLEGRO_BITMAP* bmp = al_load_bitmap(texture->name.c_str());

    if (bmp)
    {
        texture->data = bmp;
        texture->width = al_get_bitmap_width(bmp);
        texture->height = al_get_bitmap_height(bmp);
        texture->failed = false;
    }
    else
    {
        texture->data = nullptr;
        texture->failed = true;
    }
}

void Allegro::FreeTexture(Gwk::Texture* texture)
{
    al_destroy_bitmap((ALLEGRO_BITMAP*)texture->data);
    texture->data = nullptr;
}

void Allegro::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                               float u1, float v1,
                               float u2, float v2)
{
    ALLEGRO_BITMAP* bmp = (ALLEGRO_BITMAP*)texture->data;

    if (!bmp)
        return DrawMissingImage(rect);

    Translate(rect);
    const unsigned int w = texture->width;
    const unsigned int h = texture->height;
    al_draw_scaled_bitmap(bmp,
                          u1*w, v1*h, (u2-u1)*w, (v2-v1)*h,  // source
                          rect.x, rect.y, rect.w, rect.h,    // destination
                          0);
}

Gwk::Color Allegro::PixelColour(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                 const Gwk::Color& col_default)
{
    ALLEGRO_BITMAP* bmp = (ALLEGRO_BITMAP*)texture->data;

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
    al_draw_filled_rectangle(fx, fy, fx+rect.w, fy+rect.h, m_color);
}

void Allegro::DrawLinedRect(Gwk::Rect rect)
{
    Translate(rect);
    // Width of 0 draws a line, not a rect of width 1.
    const float fx = rect.x+0.5f, fy = rect.y+0.5f;
    al_draw_rectangle(fx, fy, fx+rect.w, fy+rect.h, m_color, 0.f);
}

void Allegro::DrawShavedCornerRect(Gwk::Rect rect, bool bSlight)
{
    // Draw INSIDE the w/h.
    rect.w -= 1;
    rect.h -= 1;
    
#define SET_VERT(I, X,Y)            vtx[I].x = (X), vtx[I].y = (Y), vtx[I].color = m_color
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
        al_draw_prim(vtx, nullptr, nullptr, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
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
        al_draw_prim(vtx, nullptr, nullptr, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
    }
    
#undef SET_VERT
#undef ADD_LINE
}

//  Unfortunately I think we need to lock the target texture to use this. The default
//  is that a rect of size (1,1) will be drawn.
//
//    void Allegro::DrawPixel(int x, int y)
//    {
//        al_put_pixel(x+0.5f, y+0.5f, m_color);
//    }

bool Allegro::BeginContext(Gwk::WindowProvider* window)
{
    al_clear_to_color(al_map_rgba_f(0.f, 0.f, 0.f, 0.f));
    return true;
}

bool Allegro::EndContext(Gwk::WindowProvider* window)
{
    return true;
}

bool Allegro::PresentContext(Gwk::WindowProvider* window)
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
