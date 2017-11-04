/*
 *  Software Renderer for Gwork.
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/Software.h>
#define STB_IMAGE_IMPLEMENTATION
#include <Gwork/External/stb_image.h>
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include <Gwork/External/stb_truetype.h>
#include <sys/stat.h>

namespace Gwk
{
namespace Renderer
{
    
namespace Drawing
{
    //! Blend two colors using: result = S_rgb*S_alpha + D_rgb*(1 - S_alpha)
    static inline Color BlendAlpha(Color const& src, Color const& dst)
    {
        const float a = src.a * (1.f/255.f);
        const float b = 1.f - a;
        const Color r(src.r * a + dst.r * b,
                      src.g * a + dst.g * b,
                      src.b * a + dst.b * b,
                      src.a * a + dst.a * b);
        return r;
    }

    //! Draw filled rectangle.
    void FillRect(PixelBuffer& pb, Rect const& r, Color const& c)
    {
        for (int y = 0; y < r.h; ++y)
        {
            for (int x = 0; x < r.w; ++x)
            {
                pb.At(r.x + x, r.y + y) = c;
            }
        }
    }
    
    //! Draw rectangle outline.
    void OutlineRect(PixelBuffer& pb, Rect const& r, Color const& c)
    {
        for (int x = 0; x < r.w; ++x)
        {
            pb.At(r.x + x, r.y) = c;  // top
            if (r.h > 1)
                pb.At(r.x + x, r.y + r.h - 1) = c;  // bottom
        }
        for (int y = 0; y < r.h; ++y)
        {
            pb.At(r.x, r.y + y) = c;  // left
            if (r.w > 1)
                pb.At(r.x + r.w - 1, r.y + y) = c;  // right
        }
    }
    
    //! Draw textured rectangle.
    void TexturedRect(PixelBuffer& pb, const PixelBuffer& pbsrc,
                      const Gwk::Rect& rect, float u1, float v1, float u2, float v2)
    {
        const Point srcsz(pbsrc.GetSize());
        const Point uvtl(srcsz.x * u1, srcsz.y * v1);
        
        const float dv = (v2 - v1) * srcsz.y / rect.h;
        for (int y = 0; y < rect.h; ++y)
        {
            const float du = (u2 - u1) * srcsz.x / rect.w;
            for (int x = 0; x < rect.w; ++x)
            {
                
                pb.At(rect.x + x, rect.y + y) = BlendAlpha(pbsrc.At(uvtl.x + du*x, uvtl.y + dv*y),
                                                           pb.At(rect.x + x, rect.y + y));
            }
        }
    }
}
    
//-------------------------------------------------------------------------------

// See "Font Size in Pixels or Points" in "stb_truetype.h"
static constexpr float c_pixToPoints = 1.333f;
static constexpr int c_texsz = 256;   // TODO - fix this hack.
static PixelBuffer missing;

Font::Status SoftwareResourceLoader::LoadFont(Font& font)
{
    const String fontFile = m_paths.GetPath(ResourcePaths::Type::Font, font.facename);
    
    FILE* f = fopen(fontFile.c_str(), "rb");
    if (!f)
    {
        font.data = nullptr;
        font.status = Font::Status::ErrorFileNotFound;
        return font.status;
    }
    
    struct stat finfo;
    const int rc = stat(fontFile.c_str(), &finfo);
    const size_t fsz = rc == 0 ? finfo.st_size : -1;
    assert(fsz > 0);
    
    unsigned char* ttfdata = new unsigned char[fsz];
    fread(ttfdata, 1, fsz, f);
    fclose(f);
    
    unsigned char *font_bmp = new unsigned char[c_texsz * c_texsz];
    
    font.render_data = new stbtt_bakedchar[96];
    
    stbtt_BakeFontBitmap(ttfdata, 0,
                         font.realsize * c_pixToPoints, // height
                         font_bmp,
                         c_texsz, c_texsz,
                         32,96,             // range to bake
                         static_cast<stbtt_bakedchar*>(font.render_data));
    delete [] ttfdata;
    
    font.data = font_bmp;
    font.status = Font::Status::Loaded;
    
    return font.status;
}

void SoftwareResourceLoader::FreeFont(Gwk::Font& font)
{
    if (font.IsLoaded())
    {
        delete [] static_cast<unsigned char*>(font.data);
        delete [] static_cast<stbtt_bakedchar*>(font.render_data);
        font.status = Font::Status::Unloaded;
    }
}

Texture::Status SoftwareResourceLoader::LoadTexture(Texture& texture)
{
    if (texture.IsLoaded())
        FreeTexture(texture);
    
    const String filename = m_paths.GetPath(ResourcePaths::Type::Texture, texture.name);

    int x,y,n;
    unsigned char *data = stbi_load(filename.c_str(), &x, &y, &n, 4);
    
    if (!data)
    {
        texture.status = Texture::Status::ErrorFileNotFound;
        return texture.status;
    }
    
    auto pbuff = new PixelBuffer();
    pbuff->Init(Point(x,y));
    memcpy(&pbuff->At(0,0), data, x*y*4);
    stbi_image_free(data);

    texture.width = x;
    texture.height = y;
    texture.data = pbuff;
    texture.status = Texture::Status::Loaded;

    return texture.status;
}

void SoftwareResourceLoader::FreeTexture(Texture& texture)
{
    if (texture.IsLoaded())
    {
        delete static_cast<PixelBuffer*>(texture.data);
        texture.data = nullptr;
        texture.status = Texture::Status::Unloaded;
    }
}

//-------------------------------------------------------------------------------
    
Software::Software(ResourceLoader& loader, PixelBuffer& pbuff)
    :   Base(loader)
    ,   m_pixbuf(&pbuff)
{
    missing.Init(Point(1,1));
    missing.At(0,0) = Color(255,0,0,255);
}

Software::~Software()
{
}

void Software::SetDrawColor(Gwk::Color color)
{
    m_color = color;
}

Gwk::Point Software::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return Gwk::Point(0, 0);
    
    Point sz(0, font->realsize * c_pixToPoints);
    
    float x = 0.f, y = 0.f;
    const char *pc = text.c_str();
    size_t slen = text.length();
    
    while (slen > 0)
    {
        if (*pc >= 32 && *pc <= 127)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(static_cast<stbtt_bakedchar*>(font->render_data),
                               c_texsz,c_texsz,
                               *pc - 32,
                               &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9
            
            sz.x = q.x1;
            sz.y = std::max(sz.y, int((q.y1 - q.y0) * c_pixToPoints));
        }
        ++pc, --slen;
    }
    
    return sz;
}
    
void Software::RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return;

    int ix = pos.x, iy = pos.y;
    Translate(ix, iy);
    
    float x = ix, y = iy;

    const char *pc = text.c_str();
    size_t slen = text.length();
    const float height = font->realsize * c_pixToPoints;
    const unsigned char * const fontBmp = static_cast<const unsigned char*>(font->data);
    
    while (slen > 0)
    {
        if (*pc >= 32 && *pc <= 127)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(static_cast<stbtt_bakedchar*>(font->render_data),
                               c_texsz, c_texsz,
                               *pc - 32,
                               &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9
            
            const Rect r(q.x0, height-3 + q.y0, q.x1 - q.x0, q.y1 - q.y0);
            
            {
                const Point srcsz(c_texsz, c_texsz);
                const Point uvtl(srcsz.x * q.s0, srcsz.y * q.t0);
                Color col(m_color);
                
                const float dv = (q.t1 - q.t0) * srcsz.y / r.h;
                for (int fy = 0; fy < r.h; ++fy)
                {
                    const float du = (q.s1 - q.s0) * srcsz.x / r.w;
                    for (int fx = 0; fx < r.w; ++fx)
                    {
                        const Point fp(uvtl.x + du*fx, uvtl.y + dv*fy);
                        const unsigned char fi = fontBmp[fp.y * c_texsz + fp.x];
                        col.a = fi;
                        m_pixbuf->At(r.x + fx, r.y + fy) = Drawing::BlendAlpha(col, m_pixbuf->At(x + fx, y + fy));
                    }
                }
            }
        }
        ++pc, --slen;
    }
}

void Software::StartClip()
{
//    const Gwk::Rect &rect = ClipRegion();
}

void Software::EndClip()
{
}

void Software::DrawPixel(int x, int y)
{
    Translate(x, y);
    m_pixbuf->At(x, y) = m_color;
}

void Software::DrawFilledRect(Gwk::Rect rect)
{
    Translate(rect);
    Drawing::FillRect(*m_pixbuf, rect, m_color);
}

void Software::DrawLinedRect(Gwk::Rect rect)
{
    Translate(rect);
    Drawing::OutlineRect(*m_pixbuf, rect, m_color);
}
    
void Software::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                                float u1, float v1, float u2, float v2)
{
    Translate(rect);
    const PixelBuffer *srcbuf = static_cast<const PixelBuffer*>(texture->data);
    if (srcbuf != nullptr)
    {
        Drawing::TexturedRect(*m_pixbuf, *srcbuf, rect, u1,v1, u2,v2);
    }
    else
    {
        Drawing::TexturedRect(*m_pixbuf, missing, rect, 0.f,0.f, 1.f,1.f);
    }
}

Gwk::Color Software::PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                const Gwk::Color& col_default)
{
    auto pbuff = static_cast<PixelBuffer*>(texture->data);
    const Gwk::Color col = pbuff->At(x, y);
    return col;
}
    

} // Renderer
} // Gwork
