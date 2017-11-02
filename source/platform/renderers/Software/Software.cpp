/*
 *  Software Renderer for Gwork.
 *  Copyright (c) 2013-2017 Nick Trout
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/Software.h>
#define STB_IMAGE_IMPLEMENTATION
#include <Gwork/External/stb_image.h>

namespace Gwk
{
namespace Renderer
{
    
namespace Drawing
{
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
    void DrawRect(PixelBuffer& pb, Rect const& r, Color const& c)
    {
        for (int x = 0; x < r.w; ++x)
        {
            pb.At(r.x + x, r.y) = c;  // top
            pb.At(r.x + x, r.y + r.h - 1) = c;  // bottom
        }
        for (int y = 0; y < r.h; ++y)
        {
            pb.At(r.x, r.y + y) = c;  // left
            pb.At(r.x + r.w - 1, r.y + y) = c;  // right
        }
    }
    
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
                pb.At(rect.x + x, rect.y + y) = pbsrc.At(uvtl.x + du*x, uvtl.y + dv*y);
            }
        }
    }
}
    
//-------------------------------------------------------------------------------

Font::Status SoftwareResourceLoader::LoadFont(Font& font)
{
    const String fontFile = m_paths.GetPath(ResourcePaths::Type::Font, font.facename);
    
    return font.status;
}

void SoftwareResourceLoader::FreeFont(Gwk::Font& font)
{
    if (font.status == Font::Status::Loaded)
    {
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
}

Software::~Software()
{
}

void Software::SetDrawColor(Gwk::Color color)
{
    m_color = color;
}

void Software::RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return;    
}

Gwk::Point Software::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return Gwk::Point(0, 0);

    return Point(0,0);
}

void Software::StartClip()
{
    const Gwk::Rect &rect = ClipRegion();
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
    Drawing::DrawRect(*m_pixbuf, rect, m_color);
}
    
void Software::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect,
                                float u1, float v1, float u2, float v2)
{
    Translate(rect);
    const PixelBuffer *srcbuf = static_cast<const PixelBuffer*>(texture->data);
    Drawing::TexturedRect(*m_pixbuf, *srcbuf, rect, u1,v1, u2,v2);
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
