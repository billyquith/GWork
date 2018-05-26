/*
*  Gwork
*  Copyright (c) 2010 Facepunch Studios
*  Copyright (c) 2013-2018 Billy Quith
*  See license in Gwork.h
*/

#include <windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include <Gwork/Renderers/Direct2D.h>
#include <Gwork/Utility.h>

namespace Gwk
{
namespace Renderer
{

struct FontData
{
    IDWriteTextFormat*  textFormat;
};

struct TextureData
{
    ID2D1Bitmap*        bitmap; // device-specific
    IWICBitmapSource*   wICBitmap;
};

Font::Status Direct2DResourceLoader::LoadFont(Font& font)
{
    const String filename = m_paths.GetPath(ResourcePaths::Type::Font, font.facename);

    IDWriteTextFormat* textFormat = nullptr;
    HRESULT hr = m_dWriteFactory->CreateTextFormat(
        Utility::Widen(filename).c_str(),
        nullptr,
        font.bold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        font.size,
        L"",
        &textFormat
    );

    if (SUCCEEDED(hr))
    {
        FontData* fontData = new FontData();
        fontData->textFormat = textFormat;
        font.data = fontData;
        font.status = Font::Status::Loaded;

        m_fontList.push_back(&font);
    }
    else
    {
        font.status = Font::Status::ErrorFileNotFound;
    }

    return font.status;
}

void Direct2DResourceLoader::FreeFont(Font& font)
{
    if (font.IsLoaded())
    {
        m_fontList.remove(&font);

        FontData* fontData = (FontData*)font.data;
        fontData->textFormat->Release();
        delete fontData;
        font.data = nullptr;

        font.status = Font::Status::Unloaded;
    }
}

Texture::Status Direct2DResourceLoader::LoadTexture(Texture& texture)
{
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* source = nullptr;
    IWICFormatConverter* converter = nullptr;
    ID2D1Bitmap* d2DBitmap = nullptr;

    const String filename = m_paths.GetPath(ResourcePaths::Type::Texture, texture.name);

    HRESULT hr = m_wICFactory->CreateDecoderFromFilename(
        Utility::Widen(filename).c_str(),
        nullptr,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &decoder
    );

    if (SUCCEEDED(hr))
        hr = decoder->GetFrame(0, &source);

    if (SUCCEEDED(hr))
    {
        // Convert the image format to 32bppPBGRA
        // (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
        hr = m_wICFactory->CreateFormatConverter(&converter);
    }

    if (SUCCEEDED(hr))
    {
        hr = converter->Initialize(
            source,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.f,
            WICBitmapPaletteTypeMedianCut
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = m_rT->CreateBitmapFromWicBitmap(
            converter,
            nullptr,
            &d2DBitmap
        );
    }

    if (SUCCEEDED(hr))
    {
        TextureData* texdata = new TextureData();
        texdata->wICBitmap = source;
        texdata->bitmap = d2DBitmap;
        texture.data = texdata;

        D2D1_SIZE_F size = texdata->bitmap->GetSize();
        texture.width = size.width;
        texture.height = size.height;

        texture.status = Texture::Status::Loaded;
    }
    else
    {
        texture.status = Texture::Status::ErrorBadData;
    }

    if (decoder != nullptr)
        decoder->Release();

    if (converter != nullptr)
        converter->Release();

    m_textureList.push_back(&texture);

    return texture.status;
}

void Direct2DResourceLoader::FreeTexture(Texture& texture)
{
    if (texture.IsLoaded())
    {
        m_textureList.remove(&texture);

        if (texture.data != nullptr)
        {
            TextureData* texdata = (TextureData*)texture.data;

            if (texdata->wICBitmap != nullptr)
                texdata->wICBitmap->Release();

            if (texdata->bitmap != nullptr)
                texdata->bitmap->Release();

            delete texdata;
        }

        texture.data = nullptr;

    }
}

void Direct2DResourceLoader::Notify(NotificationType msg)
{
    switch (msg)
    {
    case ResourceLoader::NotificationType::DeviceLost:
        // We lost the device so free the texture resources, but remember what we lost.
        m_texturesLost.clear();
        while (!m_textureList.empty())
        {
            Texture* tex = m_textureList.front();
            m_texturesLost.push_back(tex);
            FreeTexture(*tex);
        }
        break;

    case ResourceLoader::NotificationType::DeviceAcquired:
        // Reload any resources we lost.
        for (auto&& tex : m_texturesLost)
        {
            LoadTexture(*tex);
        }
        m_texturesLost.clear();
        break;

    case ResourceLoader::NotificationType::ReleaseResources:
        while (!m_textureList.empty())
        {
            FreeTexture(*m_textureList.front());
        }
        while (!m_fontList.empty())
        {
            FreeFont(*m_fontList.front());
        }
        break;

    default:
        ;
    }

}

Direct2D::Direct2D(ResourceLoader& loader, ID2D1RenderTarget* rT, IDWriteFactory* dWriteFactory)
    :   Base(loader)
    ,   m_rT(nullptr)
    ,   m_dWriteFactory(dWriteFactory)
    ,   m_color(D2D1::ColorF::White)
{
    DeviceAcquired(rT);
}

Direct2D::~Direct2D()
{
}

void Direct2D::Begin()
{
}

void Direct2D::End()
{
}

void Direct2D::DrawFilledRect(Gwk::Rect rect)
{
    Translate(rect);

    if (m_solidColorBrush)
    {
        m_rT->FillRectangle(D2D1::RectF(rect.x, rect.y, rect.x+rect.w, rect.y+rect.h), m_solidColorBrush);
    }
}

void Direct2D::SetDrawColor(Gwk::Color color)
{
    m_color = D2D1::ColorF(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
    m_solidColorBrush->SetColor(m_color);
}

void Direct2D::RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text)
{
    FontData* fontData = (FontData*)font->data;
    Translate(pos.x, pos.y);

    if (m_solidColorBrush)
    {
        auto const wstr(Gwk::Utility::Widen(text));
        m_rT->DrawText(wstr.c_str(), wstr.length(),
                       fontData->textFormat,
                       D2D1::RectF(pos.x, pos.y, pos.x+50000, pos.y+50000),
                       m_solidColorBrush);
    }
}

Gwk::Point Direct2D::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    if (!EnsureFont(*font))
        return Gwk::Point(0, 0);

    FontData* fontData = (FontData*)font->data;
    Gwk::Point size;
    IDWriteTextLayout* layout;
    DWRITE_TEXT_METRICS metrics;
    m_dWriteFactory->CreateTextLayout(Utility::Widen(text).c_str(),
                                      static_cast<UINT32>(text.length()),
                                      fontData->textFormat,
                                      50000, 50000,
                                      &layout);
    layout->GetMetrics(&metrics);
    layout->Release();
    return Gwk::Point(metrics.widthIncludingTrailingWhitespace, metrics.height);
}

void Direct2D::DeviceLost()
{
    if (m_solidColorBrush != nullptr)
    {
        m_solidColorBrush->Release();
        m_solidColorBrush = nullptr;
    }

    GetLoader().Notify(ResourceLoader::NotificationType::DeviceLost);
}

void Direct2D::DeviceAcquired(ID2D1RenderTarget* rT)
{
    m_rT = rT;
    HRESULT hr = m_rT->CreateSolidColorBrush(m_color, &m_solidColorBrush);

    GetLoader().Notify(ResourceLoader::NotificationType::DeviceAcquired);
}

void Direct2D::StartClip()
{
    Gwk::Rect rect = ClipRegion();
    D2D1_RECT_F r =
        D2D1::RectF(rect.x*Scale(), rect.y*Scale(),
                    (rect.x+rect.w)*Scale(), (rect.y+rect.h)*Scale());
    m_rT->PushAxisAlignedClip(r, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
}

void Direct2D::EndClip()
{
    m_rT->PopAxisAlignedClip();
}

void Direct2D::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect, float u1,
                                float v1, float u2, float v2)
{
    TextureData* texData = (TextureData*)texture->data;

    // Missing image, not loaded properly?
    if (!texData || texData->bitmap == nullptr)
        return DrawMissingImage(rect);

    Translate(rect);
    m_rT->DrawBitmap(texData->bitmap,
                      D2D1::RectF(rect.x, rect.y, rect.x+rect.w, rect.y+rect.h),
                      1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
                      D2D1::RectF(u1*texture->width, v1*texture->height, u2*
                                  texture->width, v2*texture->height)
                      );
}

Gwk::Color Direct2D::PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                const Gwk::Color& col_default)
{
    TextureData* texData = (TextureData*)texture->data;

    if (!texData || texData->bitmap == nullptr)
        return col_default;

    WICRect sourceRect;
    sourceRect.X = x;
    sourceRect.Y = y;
    sourceRect.Width = sourceRect.Height = 1;

    // these bitmaps are always in GUID_WICPixelFormat32bppPBGRA
    byte pixelBuffer[4*1*1];
    texData->wICBitmap->CopyPixels(&sourceRect, 4, 4 * texture->width * texture->height, pixelBuffer);
    return Gwk::Color(pixelBuffer[2], pixelBuffer[1], pixelBuffer[0], pixelBuffer[3]);
}

void Direct2D::DrawLinedRect(Gwk::Rect rect)
{
    Translate(rect);

    if (m_solidColorBrush)
    {
        m_rT->DrawRectangle(D2D1::RectF(rect.x, rect.y, rect.x+rect.w,
                                         rect.y+rect.h), m_solidColorBrush);
    }
}

void Direct2D::DrawShavedCornerRect(Gwk::Rect rect, bool bSlight)
{
    Translate(rect);

    if (m_solidColorBrush)
    {
        m_rT->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(rect.x, rect.y, rect.x+rect.w, rect.y+rect.h), 10.f, 10.f),
                                   m_solidColorBrush);
    }
}

void Direct2D::Release()
{
    GetLoader().Notify(ResourceLoader::NotificationType::ReleaseResources);
}

bool Direct2D::InternalCreateDeviceResources()
{
    HRESULT hr = S_OK;

    if (!m_rT)
    {
        RECT rc;
        GetClientRect(m_hWND, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(
            rc.right-rc.left,
            rc.bottom-rc.top
            );
        ID2D1HwndRenderTarget* rT;
        // Create a Direct2D render target.
        hr = m_d2DFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hWND, size),
            &rT
            );

        if (SUCCEEDED(hr))
        {
            rT->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
            DeviceAcquired(rT);
        }
    }

    return SUCCEEDED(hr);
}

void Direct2D::InternalReleaseDeviceResources()
{
    if (m_rT != nullptr)
    {
        m_rT->Release();
        m_rT = nullptr;
    }
}

bool Direct2D::InitializeContext(Gwk::WindowProvider* window)
{
    //m_hWND = (HWND)window->GetWindow();
    //HRESULT hr = D2D1CreateFactory(
    //    D2D1_FACTORY_TYPE_SINGLE_THREADED,
    //    &m_d2DFactory
    //    );

    //if (FAILED(hr))
    //    return false;

    //hr = DWriteCreateFactory(
    //    DWRITE_FACTORY_TYPE_SHARED,
    //    __uuidof(IDWriteFactory),
    //    reinterpret_cast<IUnknown**>(&m_dWriteFactory)
    //    );

    //if (FAILED(hr))
    //    return false;

    //hr = CoInitialize(nullptr);

    //if (FAILED(hr))
    //    return false;

    //hr = CoCreateInstance(
    //    CLSID_WICImagingFactory,
    //    nullptr,
    //    CLSCTX_INPROC_SERVER,
    //    IID_IWICImagingFactory,
    //    reinterpret_cast<void**>(&m_wICFactory)
    //    );

    //if (FAILED(hr))
    //    return false;

    //return InternalCreateDeviceResources();
    return false;
}

bool Direct2D::ShutdownContext(Gwk::WindowProvider* window)
{
    //InternalReleaseDeviceResources();
    //DeviceLost();
    return true;
}

bool Direct2D::PresentContext(Gwk::WindowProvider* window)
{
    return true;
}

bool Direct2D::ResizedContext(Gwk::WindowProvider* window, int w, int h)
{
    //HRESULT hr = ((ID2D1HwndRenderTarget*)m_rT)->Resize(D2D1::SizeU(w, h));
    //return SUCCEEDED(hr);
    return false;
}

bool Direct2D::BeginContext(Gwk::WindowProvider* window)
{
    //if (SUCCEEDED(InternalCreateDeviceResources()))
    //{
    //    m_rT->BeginDraw();
    //    return true;
    //}

    return false;
}

bool Direct2D::EndContext(Gwk::WindowProvider* window)
{
    //HRESULT hr = m_rT->EndDraw();

    //if (hr == D2DERR_RECREATE_TARGET)
    //{
    //    InternalReleaseDeviceResources();
    //    DeviceLost();
    //}

    //return SUCCEEDED(hr);
    return false;
}

}
}

} // namespace Renderer
} // namespace Gwk
