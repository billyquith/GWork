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

    GworkRender_Windows(HWND pHWND)
    {
        m_HWND = pHWND;
        m_hDC = NULL;
        m_iWidth = 0;
        m_iHeight = 0;
#ifdef USE_GDIPLUS_DOUBLE_BUFFERING
        m_Bitmap = NULL;
        m_CachedBitmap = NULL;
#endif
        graphics = NULL;
        // Initialize GDI+.
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
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
        GetClientRect(m_HWND, &rect);
        int width = rect.right-rect.left;
        int height = rect.bottom-rect.top;

        if (m_iWidth != width)
            DestroyOffscreenBitmap();

        if (m_iHeight != height)
            DestroyOffscreenBitmap();

#ifdef USE_GDIPLUS_DOUBLE_BUFFERING

        if (m_Bitmap)
            return;

        m_iWidth = width;
        m_iHeight = height;
        Graphics gfx(m_hDC);
        m_Bitmap = new Bitmap(m_iWidth, m_iHeight, &gfx);
        graphics = Graphics::FromImage(m_Bitmap);
#endif
    }

    void DestroyOffscreenBitmap()
    {
#ifdef USE_GDIPLUS_DOUBLE_BUFFERING

        if (m_Bitmap)
        {
            delete m_Bitmap; m_Bitmap = NULL;
        }

        graphics = NULL;
#endif
    }

    virtual void Begin()
    {
        m_hDC = BeginPaint(m_HWND, &m_PaintStruct);
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
        gfx.DrawImage(m_Bitmap, 0, 0);
#endif
#ifndef USE_GDIPLUS_DOUBLE_BUFFERING

        if (graphics)
        {
            delete graphics;
            graphics = NULL;
        }

#endif
        m_hDC = NULL;
        EndPaint(m_HWND, &m_PaintStruct);
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
        //	IGet::Render()->PushMaterial((IMaterial*)material);
    }

    virtual void PopMaterial(void)
    {
        //	IGet::Render()->PopMaterial();
    }

    // Why is this here
    virtual const char* GetMaterial(void)
    {
        return "";
    }

    virtual void* ImagePointer(const char* image)
    {
        //	return IGet::Render()->LoadMaterial( image );
        return NULL;
    }

    virtual void SetDrawColor(Gwk::Color color)
    {
        m_Col = Color(color.a, color.r, color.g, color.b);
    }

    Color GetGDIColor()
    {
        return m_Col;
    }

    virtual void LoadFont(Gwk::Font* font)
    {
        Gwk::Debug::Msg("LOAD FONT %s\n", font->facename.c_str());
        FontStyle fs = FontStyleRegular;
        font->realsize = font->size*Scale();
        Font* pFont = new Font(Gwk::Utility::StringToUnicode(
                                   font->facename).c_str(), font->realsize, fs, UnitPixel, NULL);
        font->data = pFont;
    }

    virtual void FreeFont(Gwk::Font* pFont)
    {
        Gwk::Debug::Msg("FREE FONT %s\n", pFont->facename.c_str());

        if (!pFont->data)
            return;

        Font* font = ((Font*)pFont->data);
        delete font;
        pFont->data = NULL;
    }

    virtual void RenderText(Gwk::Font* pFont, Gwk::Rect rect, const Gwk::UnicodeString& text)
    {
        /*
         * SetDrawColor( Gwk::Color( 255, 0, 0, 100 ) );
         * DrawFilledRect( rect );
         * SetDrawColor( Gwk::Color( 0, 0, 0, 255 ) );
         */
        Translate(rect);

        if (!pFont->data || fabs(pFont->realsize-pFont->size*Scale()) > 2)
        {
            FreeFont(pFont);
            LoadFont(pFont);
        }

        StringFormat strFormat(StringFormat::GenericDefault());
        SolidBrush solidBrush(GetGDIColor());
        RectF r(rect.x, rect.y, rect.w, rect.h);
        Font* pGDIFont = (Font*)pFont->data;
        graphics->DrawString(text.c_str(), text.length()+1, pGDIFont, r, &strFormat, &solidBrush);
    }

    virtual Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::UnicodeString& text)
    {
        Gwk::Point p(32, 32);

        if (!pFont->data || fabs(pFont->realsize-pFont->size*Scale()) > 2)
        {
            FreeFont(pFont);
            LoadFont(pFont);
        }

        StringFormat strFormat(StringFormat::GenericDefault());
        strFormat.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces|
                                 strFormat.GetFormatFlags());
        SizeF size;
        Graphics g(m_HWND);
        Font* pGDIFont = (Font*)pFont->data;
        Status s = g.MeasureString(text.c_str(), -1, pGDIFont, &strFormat, &size);
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

    bool ProcessTexture(Gwk::Texture* pTexture)
    {
        return true;
    }

    void DrawMissingImage(Gwk::Rect pTargetRect)
    {
        SetDrawColor(Gwk::Colors::Red);
        DrawFilledRect(pTargetRect);
    }

    void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect, float u1 = 0.0f,
                          float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f)
    {
        Image* pImage = (Image*)pTexture->data;

        // Missing image, not loaded properly?
        if (pImage->GetType() == ImageTypeUnknown)
            return DrawMissingImage(pTargetRect);

        Translate(pTargetRect);
        RectF TargetRect(pTargetRect.x, pTargetRect.y, pTargetRect.w, pTargetRect.h);

        if (u1 == 0.0f && v1 == 0.0f && u2 == 1.0f && v2 == 1.0f)
        {
            graphics->DrawImage(pImage, TargetRect);
            return;
        }

        float fW = pImage->GetWidth();
        float fH = pImage->GetHeight();
        u1 *= fW;
        v1 *= fH;
        u2 *= fW;
        u2 -= u1;
        v2 *= fH;
        v2 -= v1;
        graphics->DrawImage(pImage, TargetRect, u1, v1, u2, v2, UnitPixel);
    }

    void LoadTexture(Gwk::Texture* pTexture)
    {
        Gwk::Debug::Msg("LOAD TEXTURE %s\n", pTexture->name.c_str());
        Image* pImage = new Image(Gwk::Utility::StringToUnicode(pTexture->name).c_str());
        pTexture->data = pImage;
        pTexture->width = pImage->GetWidth();
        pTexture->height = pImage->GetHeight();
    }

    void FreeTexture(Gwk::Texture* pTexture)
    {
        Gwk::Debug::Msg("RELEASED TEXTURE %s\n", pTexture->name.c_str());
        Image* pImage = (Image*)pTexture->data;

        if (!pImage)
            return;

        delete pImage;
    }

private:

    Color m_Col;

    HWND m_HWND;
    HDC m_hDC;
    PAINTSTRUCT m_PaintStruct;

    int m_iWidth;
    int m_iHeight;

    ULONG_PTR m_gdiplusToken;

#ifdef USE_GDIPLUS_DOUBLE_BUFFERING
    Bitmap*         m_Bitmap;
    CachedBitmap*   m_CachedBitmap;
#endif

    Graphics*       graphics;
};
