
#include <windows.h>
#include <gdiplus.h>

#include <Gwork/Renderers/GDIPlus.h>
#include <Gwork/Utility.h>
#include <Gwork/Font.h>
#include <Gwork/Texture.h>
#include <Gwork/WindowProvider.h>

#include <math.h>

namespace Gwk
{
namespace Renderer
{
GDIPlus::GDIPlus(HWND hWND)
{
    m_hWND = hWND;
    m_hDC = nullptr;
    graphics = nullptr;
    m_width = 0;
    m_height = 0;
    // Initialize GDI+.
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
}

GDIPlus::~GDIPlus()
{
    Gdiplus::GdiplusShutdown(m_gdiplusToken);
}

void GDIPlus::Begin()
{
    m_hDC = GetDC(m_hWND);
    graphics = Gdiplus::Graphics::FromHDC(m_hDC);
}

void GDIPlus::End()
{
    if (graphics)
    {
        delete graphics;
        graphics = nullptr;
    }

    ReleaseDC(m_hWND, m_hDC);
    m_hDC = nullptr;
}

void GDIPlus::DrawFilledRect(Gwk::Rect rect)
{
    Translate(rect);
    Gdiplus::SolidBrush solidBrush(m_colour);
    graphics->FillRectangle(&solidBrush, rect.x, rect.y, rect.w, rect.h);
}

void GDIPlus::SetDrawColor(Gwk::Color color)
{
    m_colour = Gdiplus::Color(color.a, color.r, color.g, color.b);
}

void GDIPlus::LoadFont(Gwk::Font* font)
{
    Gdiplus::FontStyle fs = Gdiplus::FontStyleRegular;
    font->realsize = font->size*Scale();
    Gdiplus::Font* font = new Gdiplus::Font(Utility::Widen(font->facename).c_str(),
                                             font->realsize, fs, Gdiplus::UnitPixel, nullptr);
    font->data = font;
}

void GDIPlus::FreeFont(Gwk::Font* font)
{
    if (!font->data)
        return;

    Gdiplus::Font* font = ((Gdiplus::Font*)font->data);
    delete font;
    font->data = nullptr;
}

void GDIPlus::RenderText(Gwk::Font* font, Gwk::Point pos,
                         const Gwk::String& text)
{
    Translate(pos.x, pos.y);

    // If the font doesn't exist, or the font size should be changed
    if (!font->data || fabs(font->realsize-font->size*Scale()) > 2)
    {
        FreeFont(font);
        LoadFont(font);
    }

    Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericDefault());
    Gdiplus::SolidBrush solidBrush(m_colour);
    Gdiplus::RectF r(pos.x, pos.y, 1000, 1000);
    Gdiplus::Font* gDIFont = (Gdiplus::Font*)font->data;
    const std::wstring wtext( Utility::Widen(text) );
    graphics->DrawString(wtext.c_str(), wtext.length()+1,
                         gDIFont, r, &strFormat, &solidBrush);
}

Gwk::Point GDIPlus::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    Gwk::Point p(1, 1);

    if (!font->data || fabs(font->realsize-font->size*Scale()) > 2)
    {
        FreeFont(font);
        LoadFont(font);
    }

    Gdiplus::StringFormat strFormat(Gdiplus::StringFormat::GenericDefault());
    strFormat.SetFormatFlags(
        Gdiplus::StringFormatFlagsMeasureTrailingSpaces|strFormat.GetFormatFlags());
    Gdiplus::SizeF size;
    Gdiplus::Graphics g(m_hWND);
    Gdiplus::Font* gDIFont = (Gdiplus::Font*)font->data;
    const std::wstring wtext( Utility::Widen(text) );
    g.MeasureString(wtext.c_str(), -1, gDIFont, Gdiplus::SizeF(10000, 10000),
                    &strFormat, &size);
    return Gwk::Point(size.Width+1, size.Height+1);
}

void GDIPlus::StartClip()
{
    const Gwk::Rect& rect = ClipRegion();
    graphics->SetClip(Gdiplus::Rect(rect.x*Scale(), rect.y*Scale(), rect.w*Scale(), rect.h*
                                    Scale()), Gdiplus::CombineModeReplace);
}

void GDIPlus::EndClip()
{
    graphics->ResetClip();
}

void GDIPlus::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect, float u1,
                               float v1, float u2, float v2)
{
    Gdiplus::Bitmap* image = (Gdiplus::Bitmap*)texture->data;

    // Missing image, not loaded properly?
    if (!image || image->GetType() == Gdiplus::ImageTypeUnknown)
        return DrawMissingImage(targetRect);

    Translate(targetRect);
    Gdiplus::RectF TargetRect(targetRect.x, targetRect.y, targetRect.w, targetRect.h);
    // Convert UV to pixel coords
    float fW = image->GetWidth();
    float fH = image->GetHeight();
    u1 *= fW;
    v1 *= fH;
    u2 *= fW;
    u2 -= u1;
    v2 *= fH;
    v2 -= v1;
    graphics->DrawImage(image, TargetRect, u1, v1, u2, v2, Gdiplus::UnitPixel);
}

void GDIPlus::LoadTexture(Gwk::Texture* texture)
{
    Gdiplus::Bitmap* image = new Gdiplus::Bitmap(Utility::Widen(texture->name).c_str());
    texture->data = image;
    texture->width = image->GetWidth();
    texture->height = image->GetHeight();
}

void GDIPlus::FreeTexture(Gwk::Texture* texture)
{
    Gdiplus::Bitmap* image = (Gdiplus::Bitmap*)texture->data;

    if (!image)
        return;

    delete image;
}

Gwk::Color GDIPlus::PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                 const Gwk::Color& col_default)
{
    Gdiplus::Bitmap* image = (Gdiplus::Bitmap*)texture->data;

    if (!image)
        return col_default;

    Gdiplus::Color c;
    image->GetPixel(x, y, &c);
    return Gwk::Color(c.GetR(), c.GetG(), c.GetB(), c.GetA());
}

bool GDIPlus::InitializeContext(Gwk::WindowProvider* window)
{
    m_hWND = (HWND)window->GetWindow();
    return true;
}

bool GDIPlus::ShutdownContext(Gwk::WindowProvider* window)
{
    return true;
}

bool GDIPlus::PresentContext(Gwk::WindowProvider* window)
{
    return true;
}

bool GDIPlus::ResizedContext(Gwk::WindowProvider* window, int w, int h)
{
    return true;
}

bool GDIPlus::BeginContext(Gwk::WindowProvider* window)
{
    return true;
}

bool GDIPlus::EndContext(Gwk::WindowProvider* window)
{
    return true;
}

} // namespace Renderer
} // namespace Gwk
