/*
 *  Software Renderer for Gwork.
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/Software.h>
#include <Gwork/PlatformCommon.h>
#include <Gwork/Utility.h>
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
        // use fixed point to blend
        constexpr unsigned sh = 16;
        constexpr uint32_t s = (1u << sh) / (255u);
        const uint32_t a = (src.a * s);
        const uint32_t b = (1u << sh) - a;;
        
        const Color r(((src.r * a) + (dst.r * b)) >> sh,
                      ((src.g * a) + (dst.g * b)) >> sh,
                      ((src.b * a) + (dst.b * b)) >> sh,
                      src.a);
        return r;
    }

    //! Draw filled rectangle.
    void RectFill(PixelBuffer& pb, Rect const& r, Color const& c)
    {
        for (int y = 0; y < r.h; ++y)
        {
            Color *px = &pb.At(r.x, r.y + y);
            for (int x = r.w; x > 0; --x)
            {
                *px++ = c;
            }
        }
    }

    //! Draw rectangle outline.
    void RectOutline(PixelBuffer& pb, Rect const& r, Color const& c)
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
    void RectTextured(PixelBuffer& pb, const PixelBuffer& pbsrc,
                      const Gwk::Rect& rect, float u1, float v1, float u2, float v2)
    {
        const Point srcsz(pbsrc.GetSize());
        const Point uvtl(srcsz.x * u1, srcsz.y * v1);

        const float dv = (v2 - v1) * srcsz.y / rect.h;
        float v = uvtl.y;
        for (int y = 0; y < rect.h; ++y)
        {
            const float du = (u2 - u1) * srcsz.x / rect.w;
            float u = uvtl.x;
            Color *px = &pb.At(rect.x, rect.y + y);
            for (int x = 0; x < rect.w; ++x)
            {
                *px = BlendAlpha(pbsrc.At(u, v), *px);
                ++px;
                u += du;
            }
            v += dv;
        }
    }
}

//-------------------------------------------------------------------------------

// See "Font Size in Pixels or Points" in "stb_truetype.h"
static constexpr float c_pointsToPixels = 1.333f;
static constexpr int c_texsz = 256; // arbitrary font cache texture size

class FontData : public Font::IData
{
public:
    FontData(size_t size)
    {
        font_bmp = std::unique_ptr<unsigned char[]>(new unsigned char[size]);
    }

    ~FontData()
    {
    }

    std::unique_ptr<unsigned char[]> font_bmp;
};

Font::Status SoftwareResourceLoader::LoadFont(Font& font)
{
    const String filename = m_paths.GetPath(ResourcePaths::Type::Font, font.facename);

    FILE* f = fopen(filename.c_str(), "rb");
    if (!f)
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", filename.c_str());
        font.data = nullptr;
        font.status = Font::Status::ErrorFileNotFound;
        return font.status;
    }

    struct stat finfo;
    const int rc = stat(filename.c_str(), &finfo);
    const size_t fsz = rc == 0 ? finfo.st_size : -1;
    assert(fsz > 0);

    unsigned char* ttfdata = new unsigned char[fsz];
    fread(ttfdata, 1, fsz, f);
    fclose(f);

    std::shared_ptr<FontData> fontData = std::make_shared<FontData>(c_texsz * c_texsz);

    font.render_data = new stbtt_bakedchar[96];

    stbtt_BakeFontBitmap(ttfdata, 0,
                         font.realsize * c_pointsToPixels, // height
                         fontData->font_bmp.get(),
                         c_texsz, c_texsz,
                         32,96,             // range to bake
                         static_cast<stbtt_bakedchar*>(font.render_data));
    delete [] ttfdata;

    font.data = Utility::dynamic_pointer_cast<Font::IData, FontData>(fontData);
    font.status = Font::Status::Loaded;

    return font.status;
}

void SoftwareResourceLoader::FreeFont(Gwk::Font& font)
{
    if (font.IsLoaded())
    {
        font.data.reset();
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
        Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
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
    ,   m_isClipping(false)
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

Gwk::Point Software::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return Gwk::Point(0, 0);

    Point sz(0, font->realsize * c_pointsToPixels);

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
            sz.y = std::max(sz.y, int((q.y1 - q.y0) * c_pointsToPixels));
        }
        ++pc, --slen;
    }

    return sz;
}

void Software::RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return;

    FontData* data = dynamic_cast<FontData*>(font->data.get());

    if (data == nullptr)
        return;

    int ix = pos.x, iy = pos.y;
    Translate(ix, iy);

    float x = ix, y = iy;
    auto clipRect = ClipRegion();
    const Point srcSize(c_texsz, c_texsz);

    const char *pc = text.c_str();
    size_t slen = text.length();
    const float offset = font->realsize * c_pointsToPixels * 0.8f;
    const unsigned char * const fontBmp = static_cast<const unsigned char*>(data->font_bmp.get());

    while (slen > 0)
    {
        if (*pc >= 32 && *pc <= 127)
        {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(static_cast<stbtt_bakedchar*>(font->render_data),
                               c_texsz, c_texsz,
                               *pc - 32,
                               &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

            const Rect srcCharRect(q.x0, q.y0, q.x1 - q.x0, q.y1 - q.y0);

            Color col(m_color);
            Point fpos(0, srcCharRect.y + offset);
            for (int fy = 0; fy < srcCharRect.h; ++fy, ++fpos.y)
            {
                if (fpos.y < clipRect.Top())
                    continue;

                if (fpos.y > clipRect.Bottom())
                    return;

                fpos.x = srcCharRect.x;
                for (int fx = 0; fx < srcCharRect.w; ++fx, ++fpos.x)
                {
                    if (fpos.x < clipRect.Left())
                        continue;

                    if (fpos.x > clipRect.Right())
                        return;

                    const Point srcPix(q.s0*srcSize.x + fx, q.t0*srcSize.y + fy);
                    const unsigned char fi = fontBmp[srcPix.y * srcSize.x + srcPix.x];
                    col.a = fi;

                    Color& dst = m_pixbuf->At(fpos);
                    dst = Drawing::BlendAlpha(col, dst);
                }
            }
        }
        ++pc, --slen;
    }
}

void Software::StartClip()
{
    m_isClipping = true;
}

void Software::EndClip()
{
    m_isClipping = false;
}

bool Software::Clip(Rect& rect)
{
    if (m_isClipping)
    {
        if (!ClipRegionVisible())
        {
            rect = Rect();
            return false;
        }

        const Rect& cr(ClipRegion());

        // left
        if (rect.x < cr.x)
        {
            rect.w -= cr.x - rect.x;
            rect.x = cr.x;
        }

        // top
        if (rect.y < cr.y)
        {
            rect.h -= cr.y - rect.y;
            rect.y = cr.y;
        }

        // right
        if (rect.Right() > cr.Right())
        {
            rect.w -= rect.Right() - cr.Right();
        }

        // bottom
        if (rect.Bottom() > cr.Bottom())
        {
            rect.h -= rect.Bottom() - cr.Bottom();
        }

        return rect.w > 0 && rect.h > 0;
    }

    return true;
}

void Software::DrawPixel(int x, int y)
{
    Translate(x, y);

    Rect r(x,y,1,1);
    if (Clip(r))
        m_pixbuf->At(x, y) = m_color;
}

void Software::DrawFilledRect(Gwk::Rect rect)
{
    Translate(rect);
    if (Clip(rect))
        Drawing::RectFill(*m_pixbuf, rect, m_color);
}

void Software::DrawLinedRect(Gwk::Rect rect)
{
    Translate(rect);
    if (Clip(rect))
        Drawing::RectOutline(*m_pixbuf, rect, m_color);
}

void Software::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                                float u1, float v1, float u2, float v2)
{
    Translate(rect);
    if (Clip(rect))
    {
        const PixelBuffer *srcbuf = static_cast<const PixelBuffer*>(texture->data);
        if (srcbuf != nullptr)
        {
            Drawing::RectTextured(*m_pixbuf, *srcbuf, rect, u1,v1, u2,v2);
        }
        else
        {
            DrawMissingImage(rect);
        }
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
