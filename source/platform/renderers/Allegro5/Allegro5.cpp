/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/BaseRender.h>
#include <Gwork/Renderers/Allegro5.h>
#include <Gwork/PlatformCommon.h>

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

Font::Status Allegro::LoadFont(const Font& font)
{
    FreeFont(font);
    m_lastFont = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Font, font.facename);

    ALLEGRO_FONT* afont = al_load_font(filename.c_str(),
                                       font.size * Scale(),
                                       ALLEGRO_TTF_NO_KERNING);

    if (afont)
    {
        ALFontData fontData;
        fontData.font = deleted_unique_ptr<ALLEGRO_FONT>(afont, [](ALLEGRO_FONT* mem) { if (mem) al_destroy_font(mem); });
        m_lastFont = &(*m_fonts.insert(std::make_pair(font, std::move(fontData))).first);
        return Font::Status::Loaded;
    }
    else
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", filename.c_str());
        return Font::Status::ErrorFileNotFound;
    }
}

void Allegro::FreeFont(const Gwk::Font& font)
{
    if (m_lastFont != nullptr && m_lastFont->first == font)
        m_lastFont = nullptr;

    m_fonts.erase(font); // calls ALFontData destructor
}

bool Allegro::EnsureFont(const Font& font)
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

Texture::Status Allegro::LoadTexture(const Texture& texture)
{
    FreeTexture(texture);
    m_lastTexture = nullptr;
    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

    ALLEGRO_BITMAP* bmp = al_load_bitmap(filename.c_str());

    if (bmp)
    {
        ALTextureData texData;
        texData.texture = deleted_unique_ptr<ALLEGRO_BITMAP>(bmp, [](ALLEGRO_BITMAP* mem) { if (mem) al_destroy_bitmap(mem); });
        texData.width = al_get_bitmap_width(bmp);
        texData.height = al_get_bitmap_height(bmp);
        texData.readable = false;
        m_lastTexture = &(*m_textures.insert(std::make_pair(texture, std::move(texData))).first);

        return Texture::Status::Loaded;
    }
    else
    {
        Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        return Texture::Status::ErrorFileNotFound;
    }
}

void Allegro::FreeTexture(const Gwk::Texture& texture)
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        m_lastTexture = nullptr;

    m_textures.erase(texture); // calls ALTextureData destructor
}

TextureData Allegro::GetTextureData(const Texture& texture) const
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

bool Allegro::EnsureTexture(const Gwk::Texture& texture)
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

Allegro::Allegro(ResourcePaths& paths)
:   Base(paths)
,   m_ctt(new AllegroCTT)
,   m_lastFont(nullptr)
,   m_lastTexture(nullptr)
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

void Allegro::RenderText(const Gwk::Font& font, Gwk::Point pos,
                         const Gwk::String& text)
{
    if (!EnsureFont(font))
        return;

    // at this point, the font is garented created
    auto it = m_fonts.find(font);
    // but double check :)
    if (it == m_fonts.cend())
        return;

    ALFontData& fontData = it->second;
    Translate(pos.x, pos.y);
    al_draw_text(fontData.font.get(), m_color, pos.x, pos.y, ALLEGRO_ALIGN_LEFT, text.c_str());
}

Gwk::Point Allegro::MeasureText(const Gwk::Font& font, const Gwk::String& text)
{
    if (!EnsureFont(font))
        return Gwk::Point(0, 0);

    // at this point, the font is garented created
    auto it = m_fonts.find(font);
    // but double check :)
    if (it == m_fonts.cend())
        return Gwk::Point(0, 0);

    ALFontData& fontData = it->second;

    return Point(al_get_text_width(fontData.font.get(), text.c_str()), al_get_font_line_height(fontData.font.get()));
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

void Allegro::DrawTexturedRect(const Texture& texture, Gwk::Rect rect,
                               float u1, float v1,
                               float u2, float v2)
{
    if (!EnsureTexture(texture))
        DrawMissingImage(rect);

    Translate(rect);

    ALTextureData& texData = m_lastTexture->second;

    const unsigned int w = texData.width;
    const unsigned int h = texData.height;
    al_draw_scaled_bitmap(texData.texture.get(),
                          u1*w, v1*h, (u2-u1)*w, (v2-v1)*h,  // source
                          rect.x, rect.y, rect.w, rect.h,    // destination
                          0);
}

Gwk::Color Allegro::PixelColor(const Texture& texture, unsigned int x, unsigned int y,
                                 const Gwk::Color& col_default)
{
    if (!EnsureTexture(texture))
        return col_default;

    ALTextureData& texData = m_lastTexture->second;

    ALLEGRO_COLOR col = al_get_pixel(texData.texture.get(), x, y);
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
