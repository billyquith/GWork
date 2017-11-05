#include <Gwork/BaseRender.h>
#include <Gwork/Gwork.h>
#include <Gwork/Utility.h>
#include <Gwork/Font.h>
#include <Gwork/Texture.h>

#include <windows.h>
#include <gdiplus.h>
#include <math.h>
using namespace Gdiplus;

//
// Without double buffering it's flickery
// With double buffering it gets slow at high resolutions
//
#define USE_GDIPLUS_DOUBLE_BUFFERING

class GworkRender_Windows : public Gwk::Gwk::Renderer::Base
{
public:

    GworkRender_Windows(HWND hWND)
    {
        m_hWND = hWND;
        m_hDC = nullptr;
        m_width = 0;
        m_height = 0;
#ifdef USE_GDIPLUS_DOUBLE_BUFFERING
        m_bitmap = nullptr;
        m_cachedBitmap = nullptr;
#endif
        graphics = nullptr;
        // Initialize GDI+.
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, nullptr);
    }

    ~GworkRender_Windows()
    {
#ifdef USE_GDIPLUS_DOUBLE_BUFFERING
        DestroyOffscreenBitmap();
#endif
        GdiplusShutdown(m_gdiplusToken);
    }

    void CreateOffscreenBitmap()
    {
        RECT rect;
        GetClientRect(m_hWND, &rect);
        int width = rect.right-rect.left;
        int height = rect.bottom-rect.top;

        if (m_width != width)
            DestroyOffscreenBitmap();

        if (m_height != height)
            DestroyOffscreenBitmap();

#ifdef USE_GDIPLUS_DOUBLE_BUFFERING

        if (m_bitmap)
            return;

        m_width = width;
        m_height = height;
        Graphics gfx(m_hDC);
        m_bitmap = new Bitmap(m_width, m_height, &gfx);
        graphics = Graphics::FromImage(m_bitmap);
#endif
    }

    void DestroyOffscreenBitmap()
    {
#ifdef USE_GDIPLUS_DOUBLE_BUFFERING

        if (m_bitmap)
        {
            delete m_bitmap; m_bitmap = nullptr;
        }

        graphics = nullptr;
#endif
    }

    virtual void Begin()
    {
        m_hDC = BeginPaint(m_hWND, &m_paintStruct);
        // Create the backbuffer if it doesn't exist
        // (or recreate if the size has changed)
        CreateOffscreenBitmap();
#ifndef USE_GDIPLUS_DOUBLE_BUFFERING
        graphics = Graphics::FromHDC(m_hDC);
#endif
    }

    virtual void End()
    {
#ifdef USE_GDIPLUS_DOUBLE_BUFFERING
        Graphics gfx(m_hDC);
        gfx.DrawImage(m_bitmap, 0, 0);
#endif
#ifndef USE_GDIPLUS_DOUBLE_BUFFERING

        if (graphics)
        {
            delete graphics;
            graphics = nullptr;
        }

#endif
        m_hDC = nullptr;
        EndPaint(m_hWND, &m_paintStruct);
    }

    virtual void DrawLine(int x, int y, int a, int b)
    {
        Translate(x, y);
        Translate(a, b);
        Pen pen(GetGDIColor(), 1.0f);
        graphics->DrawLine(&pen, x, y, a, b);
    }

    virtual void DrawFilledRect(Gwk::Rect rect)
    {
        Translate(rect);
        SolidBrush solidBrush(GetGDIColor());
        graphics->FillRectangle(&solidBrush, rect.x, rect.y, rect.w, rect.h);
    }

    virtual void DrawRectRotated(const Gwk::Rect& rect, float fAngle, const Gwk::Point& pntHandle)
    {
    }

    virtual void PushMaterial(const char* material)
    {
    }

    virtual void PushMaterial(const void* material)
    {
        //  IGet::Render()->PushMaterial((IMaterial*)material);
    }

    virtual void PopMaterial(void)
    {
        //  IGet::Render()->PopMaterial();
    }

    // Why is this here
    virtual const char* GetMaterial(void)
    {
        return "";
    }

    virtual void* ImagePointer(const char* image)
    {
        //  return IGet::Render()->LoadMaterial( image );
        return nullptr;
    }

    virtual void SetDrawColor(Gwk::Color color)
    {
        m_col = Color(color.a, color.r, color.g, color.b);
    }

    Color GetGDIColor()
    {
        return m_col;
    }

    virtual void LoadFont(Gwk::Font* font)
    {
        Gwk::Debug::Msg("LOAD FONT %s\n", font->facename.c_str());
        FontStyle fs = FontStyleRegular;
        font->realsize = font->size*Scale();
        Font* font = new Font(Gwk::Utility::StringToUnicode(
                                   font->facename).c_str(), font->realsize, fs, UnitPixel, nullptr);
        font->data = font;
    }

    virtual void FreeFont(Gwk::Font* font)
    {
        Gwk::Debug::Msg("FREE FONT %s\n", font->facename.c_str());

        if (!font->data)
            return;

        Font* font = ((Font*)font->data);
        delete font;
        font->data = nullptr;
    }

    virtual void RenderText(Gwk::Font* font, Gwk::Rect rect, const Gwk::UnicodeString& text)
    {
        /*
         * SetDrawColor( Gwk::Color( 255, 0, 0, 100 ) );
         * DrawFilledRect( rect );
         * SetDrawColor( Gwk::Color( 0, 0, 0, 255 ) );
         */
        Translate(rect);

        if (!font->data || fabs(font->realsize-font->size*Scale()) > 2)
        {
            FreeFont(font);
            LoadFont(font);
        }

        StringFormat strFormat(StringFormat::GenericDefault());
        SolidBrush solidBrush(GetGDIColor());
        RectF r(rect.x, rect.y, rect.w, rect.h);
        Font* gDIFont = (Font*)font->data;
        graphics->DrawString(text.c_str(), text.length()+1, gDIFont, r, &strFormat, &solidBrush);
    }

    virtual Gwk::Point MeasureText(Gwk::Font* font, const Gwk::UnicodeString& text)
    {
        Gwk::Point p(32, 32);

        if (!font->data || fabs(font->realsize-font->size*Scale()) > 2)
        {
            FreeFont(font);
            LoadFont(font);
        }

        StringFormat strFormat(StringFormat::GenericDefault());
        strFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces|
                                 strFormat.GetFormatFlags());
        SizeF size;
        Graphics g(m_hWND);
        Font* gDIFont = (Font*)font->data;
        Status s = g.MeasureString(text.c_str(), -1, gDIFont, &strFormat, &size);
        return Gwk::Point(size.Width+1, size.Height+1);
    }

    void StartClip()
    {
        const Gwk::Rect& rect = ClipRegion();
        graphics->SetClip(Rect(rect.x*Scale(), rect.y*Scale(), rect.w*Scale(),
                               rect.h*Scale()), CombineMode::CombineModeReplace);
        // Pen      pen( Color( 100, 255, 0, 255 ) );
        // graphics->DrawRectangle( &pen, Rect( rect.x*Scale(), rect.y*Scale(),
        // rect.w*Scale(), rect.h*Scale() ) );
    }

    void EndClip()
    {
        graphics->ResetClip();
    }

    bool ProcessTexture(Gwk::Texture* texture)
    {
        return true;
    }

    void DrawMissingImage(Gwk::Rect targetRect)
    {
        SetDrawColor(Gwk::Colors::Red);
        DrawFilledRect(targetRect);
    }

    void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect, float u1 = 0.0f,
                          float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        Image* image = (Image*)texture->data;

        // Missing image, not loaded properly?
        if (image->GetType() == ImageTypeUnknown)
            return DrawMissingImage(targetRect);

        Translate(targetRect);
        RectF TargetRect(targetRect.x, targetRect.y, targetRect.w, targetRect.h);

        if (u1 == 0.0f && v1 == 0.0f && u2 == 1.0f && v2 == 1.0f)
        {
            graphics->DrawImage(image, TargetRect);
            return;
        }

        float fW = image->GetWidth();
        float fH = image->GetHeight();
        u1 *= fW;
        v1 *= fH;
        u2 *= fW;
        u2 -= u1;
        v2 *= fH;
        v2 -= v1;
        graphics->DrawImage(image, TargetRect, u1, v1, u2, v2, UnitPixel);
    }

    void LoadTexture(Gwk::Texture* texture)
    {
        Gwk::Debug::Msg("LOAD TEXTURE %s\n", texture->name.c_str());
        Image* image = new Image(Gwk::Utility::StringToUnicode(texture->name).c_str());
        texture->data = image;
        texture->width = image->GetWidth();
        texture->height = image->GetHeight();
    }

    void FreeTexture(Gwk::Texture* texture)
    {
        Gwk::Debug::Msg("RELEASED TEXTURE %s\n", texture->name.c_str());
        Image* image = (Image*)texture->data;

        if (!image)
            return;

        delete image;
    }

private:

    Color m_col;

    HWND m_hWND;
    HDC m_hDC;
    PAINTSTRUCT m_paintStruct;

    int m_width;
    int m_height;

    ULONG_PTR m_gdiplusToken;

#ifdef USE_GDIPLUS_DOUBLE_BUFFERING
    Bitmap*         m_bitmap;
    CachedBitmap*   m_cachedBitmap;
#endif

    Graphics*       graphics;
};
