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

#include <fstream>

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
    template <typename T>
    void RectFill(T& pb, Rect const& r, Color const& c)
    {
        for (int y = 0; y < r.h; ++y)
        {
            Color *px = &pb.At(r.x, r.y + y);
            for (int x = r.w; x > 0; --x)
            {
                *px = BlendAlpha(c, *px);
                ++px;
            }
        }
    }

    //! Draw rectangle outline.
    template <typename T>
    void RectOutline(T& pb, Rect const& r, Color const& c)
    {
        for (int x = 0; x < r.w; ++x)
        {
            pb.At(r.x + x, r.y) = BlendAlpha(c, pb.At(r.x + x, r.y));  // top
            if (r.h > 1)
                pb.At(r.x + x, r.y + r.h - 1) = BlendAlpha(c, pb.At(r.x + x, r.y + r.h - 1));  // bottom
        }
        for (int y = 0; y < r.h; ++y)
        {
            pb.At(r.x, r.y + y) = BlendAlpha(c, pb.At(r.x, r.y + y));  // left
            if (r.w > 1)
                pb.At(r.x + r.w - 1, r.y + y) = BlendAlpha(c, pb.At(r.x + r.w - 1, r.y + y));  // right
        }
    }

    //! Draw textured rectangle.
    template <typename T, typename U>
    void RectTextured(T& pb, const U& pbsrc,
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
// arbitrary font cache texture size
// Texture size too small for wchar_t characters but stbtt_BakeFontBitmap crashes on larger sizes
static constexpr int c_texsz = 800;

Font::Status Software::LoadFont(const Font& font)
{
    FreeFont(font);
    m_lastFont = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Font, font.facename);

    std::ifstream inFile(filename, std::ifstream::in | std::ifstream::binary);

    if (!inFile.good())
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", filename.c_str());
        return Font::Status::ErrorFileNotFound;
    }

    std::streampos begin = inFile.tellg();
    inFile.seekg(0, std::ios::end);
    const size_t fsz = inFile.tellg() - begin;
    inFile.seekg(0, std::ios::beg);
    assert(fsz > 0);

    auto ttfdata = std::unique_ptr<unsigned char[]>(new unsigned char[fsz]);
    inFile.read(reinterpret_cast<char*>(ttfdata.get()), fsz);
    inFile.close();

    SWFontData fontData;
    fontData.m_ReadData =
        deleted_unique_ptr<unsigned char>(new unsigned char[c_texsz * c_texsz],
                                          [](unsigned char* mem) { delete [] mem; });
    fontData.width = c_texsz;
    fontData.height = c_texsz;

    const float realsize = font.size * Scale();
    fontData.baked_chars.resize(LastCharacter - BeginCharacter + 1);

    stbtt_BakeFontBitmap(ttfdata.get(), 0,
        realsize * c_pointsToPixels, // height
        fontData.m_ReadData.get(),
        c_texsz, c_texsz,
        BeginCharacter, LastCharacter,             // range to bake
        reinterpret_cast<stbtt_bakedchar*>(fontData.baked_chars.data()));

    m_lastFont = &(*m_fonts.insert(std::make_pair(font, std::move(fontData))).first);
    return Font::Status::Loaded;
}

void Software::FreeFont(const Gwk::Font& font)
{
    if (m_lastFont != nullptr && m_lastFont->first == font)
        m_lastFont = nullptr;

    m_fonts.erase(font); // calls SWFontData destructor
}

bool Software::EnsureFont(const Font& font)
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

Texture::Status Software::LoadTexture(const Texture& texture)
{
    FreeTexture(texture);
    m_lastTexture = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

    SWTextureData texData;

    int width, height, n;
    {
        unsigned char *image = stbi_load(filename.c_str(), &width, &height, &n, 4);
        texData.m_ReadData =
            deleted_unique_ptr<unsigned char>(image,
                                              [](unsigned char* mem) {
                                                  if (mem) stbi_image_free(mem);
                                              });
    }

    // Image failed to load..
    if (!texData.m_ReadData)
    {
        Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        return Texture::Status::ErrorFileNotFound;
    }

    texData.readable = true;

    texData.width = width;
    texData.height = height;

    m_lastTexture = &(*m_textures.insert(std::make_pair(texture, std::move(texData))).first);
    return Texture::Status::Loaded;
}

void Software::FreeTexture(const Gwk::Texture& texture)
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        m_lastTexture = nullptr;

    m_textures.erase(texture); // calls SWTextureData destructor
}

TextureData Software::GetTextureData(const Texture& texture) const
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

bool Software::EnsureTexture(const Gwk::Texture& texture)
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

Software::Software(ResourcePaths& paths, PixelBuffer& pbuff)
    :   Base(paths)
    ,   m_lastFont(nullptr)
    ,   m_lastTexture(nullptr)
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

Gwk::Point Software::MeasureText(const Gwk::Font& font, const Gwk::String& text)
{
    if (!EnsureFont(font))
        return Gwk::Point(0, 0);

    SWFontData& fontData = m_lastFont->second;

    Point sz(0, font.size * Scale() * c_pointsToPixels);

    float x = 0.f, y = 0.f;
    char* text_ptr = const_cast<char*>(text.c_str());

    while (const auto wide_char = Utility::Strings::utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;

        if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(reinterpret_cast<stbtt_bakedchar*>(fontData.baked_chars.data()),
                           c_texsz, c_texsz,
                           c,
                           &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

        sz.x = q.x1;
        sz.y = std::max(sz.y, int((q.y1 - q.y0) * c_pointsToPixels));
    }

    return sz;
}

void Software::RenderText(const Gwk::Font& font, Gwk::Point pos,
    const Gwk::String& text)
{
    if (!EnsureFont(font))
        return;
    
    SWFontData& fontData = m_lastFont->second;

    float x = pos.x, y = pos.y;
    char* text_ptr = const_cast<char*>(text.c_str());
    const auto clipRect = ClipRegion();
    const Point srcSize(fontData.width, fontData.height);
    const unsigned char * const fontBmp = fontData.m_ReadData.get();

    // Height of font, allowing for descenders, because baseline is bottom of capitals.
    const float offset = font.size * Scale() * c_pointsToPixels * 0.8f;

    Color col(m_color);
    while (const auto wide_char = Utility::Strings::utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;

        if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(reinterpret_cast<stbtt_bakedchar*>(fontData.baked_chars.data()),
            c_texsz, c_texsz,
            c,
            &x, &y, &q, 1); // 1=opengl & d3d10+,0=d3d9

        Rect srcCharRect(q.x0, q.y0, q.x1 - q.x0, q.y1 - q.y0);
        Translate(srcCharRect);
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

void Software::DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect rect,
                                float u1, float v1, float u2, float v2)
{
    if (!EnsureTexture(texture))
        return DrawMissingImage(rect);

    const SWTextureData& texData = m_lastTexture->second;

    Translate(rect);
    if (!Clip(rect))
        return;

    Drawing::RectTextured<PixelBuffer, SWTextureData>(*m_pixbuf, texData, rect, u1,v1, u2,v2);
}

Gwk::Color Software::PixelColor(const Gwk::Texture& texture, unsigned int x, unsigned int y,
                                const Gwk::Color& col_default)
{
    if (!EnsureTexture(texture))
        return col_default;

    const SWTextureData& texData = m_lastTexture->second;

    return texData.At(x, y);
}


} // Renderer
} // Gwork
