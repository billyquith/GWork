
#include <Gwork/Renderers/DirectX9.h>
#include <Gwork/Utility.h>
#include <Gwork/Font.h>
#include <Gwork/Texture.h>
#include <Gwork/WindowProvider.h>

#define D3DFVF_VERTEXFORMAT2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct FontData
{
    ID3DXFont*  font;
    int iSpaceWidth;
};


namespace Gwk
{
namespace Renderer
{
    
DirectX9::DirectX9(IDirect3DDevice9* device)
{
    m_D3D = nullptr;
    m_device = device;
    m_vertNum = 0;

    for (int i = 0; i < MaxVerts; i++)
    {
        m_verts[ i ].z = 0.5f;
        m_verts[ i ].rhw = 1.0f;
    }
}

DirectX9::~DirectX9()
{
}

void DirectX9::Begin()
{
    m_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    m_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
    m_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
    m_device->SetTextureStageState(0, D3DTSS_COLOROP,     D3DTOP_MODULATE);
    m_device->SetTextureStageState(0, D3DTSS_COLORARG1,   D3DTA_TEXTURE);
    m_device->SetTextureStageState(0, D3DTSS_COLORARG2,   D3DTA_CURRENT);
    m_device->SetTextureStageState(0, D3DTSS_ALPHAOP,     D3DTOP_MODULATE);
    m_device->SetTextureStageState(0, D3DTSS_ALPHAARG1,   D3DTA_TEXTURE);
    m_device->SetTextureStageState(0, D3DTSS_ALPHAARG2,   D3DTA_CURRENT);
    m_device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    m_device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}

void DirectX9::End()
{
    Flush();
}

void DirectX9::Flush()
{
    if (m_vertNum > 0)
    {
        m_device->SetFVF(D3DFVF_VERTEXFORMAT2D);
        m_device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, m_vertNum/3, &m_verts[0],
                                  sizeof(VertexFormat));
        m_vertNum = 0;
    }
}

void DirectX9::AddVert(int x, int y)
{
    if (m_vertNum >= MaxVerts-1)
        Flush();

    m_verts[ m_vertNum ].x = (float)x;
    m_verts[ m_vertNum ].y = (float)y;
    m_verts[ m_vertNum ].color = m_color;
    m_vertNum++;
}

void DirectX9::AddVert(int x, int y, float u, float v)
{
    if (m_vertNum >= MaxVerts-1)
        Flush();

    m_verts[ m_vertNum ].x = -0.5f+(float)x;
    m_verts[ m_vertNum ].y = -0.5f+(float)y;
    m_verts[ m_vertNum ].u = u;
    m_verts[ m_vertNum ].v = v;
    m_verts[ m_vertNum ].color = m_color;
    m_vertNum++;
}

void DirectX9::DrawFilledRect(Gwk::Rect rect)
{
    if (m_currentTexture != nullptr)
    {
        Flush();
        m_device->SetTexture(0, nullptr);
        m_currentTexture = nullptr;
    }

    Translate(rect);
    AddVert(rect.x, rect.y);
    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x, rect.y+rect.h);
    AddVert(rect.x+rect.w, rect.y);
    AddVert(rect.x+rect.w, rect.y+rect.h);
    AddVert(rect.x, rect.y+rect.h);
}

void DirectX9::SetDrawColor(Gwk::Color color)
{
    m_color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
}

void DirectX9::LoadFont(Gwk::Font* font)
{
    m_fontList.push_back(font);
    // Scale the font according to canvas
    font->realsize = font->size*Scale();
    D3DXFONT_DESC fd;
    memset(&fd, 0, sizeof(fd));
    const std::wstring wfontname( Utility::Widen(font->facename) );
    wcscpy_s(fd.FaceName, LF_FACESIZE, wfontname.c_str());
    fd.Width = 0;
    fd.MipLevels = 1;
    fd.CharSet = DEFAULT_CHARSET;
    fd.Height = font->realsize* -1.0f;
    fd.OutputPrecision = OUT_DEFAULT_PRECIS;
    fd.Italic = 0;
    fd.Weight = font->bold ? FW_BOLD : FW_NORMAL;
#ifdef CLEARTYPE_QUALITY
    fd.Quality = font->realsize < 14 ? DEFAULT_QUALITY : CLEARTYPE_QUALITY;
#else
    fd.Quality = PROOF_QUALITY;
#endif
    fd.PitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
    LPD3DXFONT d3DXFont;
    HRESULT hr = D3DXCreateFontIndirect(m_device, &fd, &d3DXFont);
    FontData*   fontData = new FontData();
    fontData->font = d3DXFont;
    // ID3DXFont doesn't measure trailing spaces, so we measure the
    // width of a space here and store it
    // in the font data - then we can add it to the width when we
    // measure text with trailing spaces.
    {
        RECT rctA = {0, 0, 0, 0};
        fontData->font->DrawTextW(nullptr, L"A", -1, &rctA,
                                    DT_CALCRECT|DT_LEFT|DT_TOP|DT_SINGLELINE, 0);
        RECT rctSpc = {0, 0, 0, 0};
        fontData->font->DrawTextW(nullptr, L"A A", -1, &rctSpc,
                                    DT_CALCRECT|DT_LEFT|DT_TOP|DT_SINGLELINE, 0);
        fontData->iSpaceWidth = rctSpc.right-rctA.right*2;
    }
    font->data = fontData;
}

void DirectX9::FreeFont(Gwk::Font* font)
{
    m_fontList.remove(font);

    if (!font->data)
        return;

    FontData* fontData = (FontData*)font->data;

    if (fontData->font)
    {
        fontData->font->Release();
        fontData->font = nullptr;
    }

    delete fontData;
    font->data = nullptr;
}

void DirectX9::RenderText(Gwk::Font* font, Gwk::Point pos,
                          const Gwk::String& text)
{
    Flush();

    // If the font doesn't exist, or the font size should be changed
    if (!font->data || fabs(font->realsize-font->size*Scale()) > 2)
    {
        FreeFont(font);
        LoadFont(font);
    }

    const std::wstring wideText(Utility::Widen(text));

    FontData* fontData = (FontData*)font->data;
    Translate(pos.x, pos.y);
    RECT ClipRect = { pos.x, pos.y, 0, 0 };
    fontData->font->DrawTextW(nullptr,
                                wideText.c_str(), -1, &ClipRect, DT_LEFT|DT_TOP|DT_NOCLIP|DT_SINGLELINE,
                                m_color);
}

Gwk::Point DirectX9::MeasureText(Gwk::Font* font, const Gwk::String& text)
{
    // If the font doesn't exist, or the font size should be changed
    if (!font->data || fabs(font->realsize-font->size*Scale()) > 2)
    {
        FreeFont(font);
        LoadFont(font);
    }

    FontData* fontData = (FontData*)font->data;
    Gwk::Point size;

    if (text.empty())
    {
        RECT rct = {0, 0, 0, 0};
        fontData->font->DrawTextW(nullptr, L"W", -1, &rct, DT_CALCRECT, 0);
        return Gwk::Point(0, rct.bottom);
    }

    const std::wstring wideText(Utility::Widen(text));

    RECT rct = {0, 0, 0, 0};
    fontData->font->DrawTextW(nullptr,
                                wideText.c_str(), -1, &rct, DT_CALCRECT|DT_LEFT|DT_TOP|DT_SINGLELINE,
                                0);

    for (int i = wideText.length()-1; i >= 0 && wideText[i] == L' '; i--)
    {
        rct.right += fontData->iSpaceWidth;
    }

    return Gwk::Point(rct.right/Scale(), rct.bottom/Scale());
}

void DirectX9::StartClip()
{
    Flush();
    m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    const Gwk::Rect& rect = ClipRegion();
    RECT r;
    r.left = ceil(((float)rect.x)*Scale());
    r.right = ceil(((float)(rect.x+rect.w))*Scale());
    r.top = ceil((float)rect.y*Scale());
    r.bottom = ceil(((float)(rect.y+rect.h))*Scale());
    m_device->SetScissorRect(&r);
}

void DirectX9::EndClip()
{
    Flush();
    m_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

void DirectX9::DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect, float u1,
                                float v1, float u2, float v2)
{
    IDirect3DTexture9* image = (IDirect3DTexture9*)texture->data;

    // Missing image, not loaded properly?
    if (!image)
        return DrawMissingImage(rect);

    Translate(rect);

    if (m_currentTexture != image)
    {
        Flush();
        m_device->SetTexture(0, image);
        m_currentTexture = image;
    }

    AddVert(rect.x, rect.y,            u1, v1);
    AddVert(rect.x+rect.w, rect.y,       u2, v1);
    AddVert(rect.x, rect.y+rect.h,   u1, v2);
    AddVert(rect.x+rect.w, rect.y,       u2, v1);
    AddVert(rect.x+rect.w, rect.y+rect.h, u2, v2);
    AddVert(rect.x, rect.y+rect.h, u1, v2);
}

void DirectX9::LoadTexture(Gwk::Texture* texture)
{
    IDirect3DTexture9* ptr = nullptr;
    D3DXIMAGE_INFO ImageInfo;
    const std::wstring wtexName( Utility::Widen(texture->name) );
    HRESULT hr = D3DXCreateTextureFromFileExW(m_device,
                                              wtexName.c_str(),
                                              0, 0, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                              D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, &ImageInfo, nullptr,
                                              &ptr);

    if (hr != S_OK)
        return;

    texture->data = ptr;
    texture->width = ImageInfo.Width;
    texture->height = ImageInfo.Height;
}

void DirectX9::FreeTexture(Gwk::Texture* texture)
{
    IDirect3DTexture9* image = (IDirect3DTexture9*)texture->data;

    if (!image)
        return;

    image->Release();
    texture->data = nullptr;
}

Gwk::Color DirectX9::PixelColour(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default)
{
    IDirect3DTexture9* image = (IDirect3DTexture9*)texture->data;

    if (!image)
        return col_default;

    IDirect3DSurface9* surface = nullptr;

    if (image->GetSurfaceLevel(0, &surface) != S_OK)
        return col_default;

    if (!surface)
        return col_default;

    D3DLOCKED_RECT lockedRect;
    surface->LockRect(&lockedRect, nullptr, D3DLOCK_READONLY);
    DWORD* pixels = (DWORD*)lockedRect.bits;
    D3DXCOLOR color = pixels[lockedRect.Pitch/sizeof(DWORD)*y+x];
    surface->UnlockRect();
    surface->Release();
    return Gwk::Color(color.r*255, color.g*255, color.b*255, color.a*255);
}

void DirectX9::Release()
{
    Font::List::iterator it = m_fontList.begin();

    while (it != m_fontList.end())
    {
        FreeFont(*it);
        it = m_fontList.begin();
    }
}

void DirectX9::FillPresentParameters(Gwk::WindowProvider* window,
                                     D3DPRESENT_PARAMETERS& Params)
{
    HWND hWND = (HWND)window->GetWindow();
    RECT ClientRect;
    GetClientRect(hWND, &ClientRect);
    ZeroMemory(&Params, sizeof(Params));
    Params.Windowed                     = true;
    Params.SwapEffect                   = D3DSWAPEFFECT_DISCARD;
    Params.BackBufferWidth              = ClientRect.right;
    Params.BackBufferHeight             = ClientRect.bottom;
    Params.FullScreen_RefreshRateInHz   = D3DPRESENT_RATE_DEFAULT;
    Params.BackBufferFormat             = D3DFMT_X8R8G8B8;
    Params.PresentationInterval         = D3DPRESENT_INTERVAL_IMMEDIATE;
}

bool DirectX9::InitializeContext(Gwk::WindowProvider* window)
{
    HWND hWND = (HWND)window->GetWindow();
    m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

    if (!m_D3D)
        return false;

    D3DCAPS9 D3DCaps;
    m_D3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps);
    DWORD BehaviourFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

    if (D3DCaps.VertexProcessingCaps != 0)
        BehaviourFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

    D3DPRESENT_PARAMETERS Params;
    FillPresentParameters(window, Params);
    HRESULT hr = m_D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWND,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING, &Params,
                                      &m_device);

    if (FAILED(hr))
        return false;

    return true;
}

bool DirectX9::ShutdownContext(Gwk::WindowProvider* window)
{
    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }

    if (m_D3D)
    {
        m_D3D->Release();
        m_D3D = nullptr;
    }

    return true;
}

bool DirectX9::PresentContext(Gwk::WindowProvider* window)
{
    m_device->Present(nullptr, nullptr, nullptr, nullptr);
    return true;
}

bool DirectX9::ResizedContext(Gwk::WindowProvider* window, int w, int h)
{
    // Force setting the current texture again
    m_currentTexture = nullptr;
    // Free any unmanaged resources (fonts)
    Release();
    // Get the new window size from the HWND
    D3DPRESENT_PARAMETERS Params;
    FillPresentParameters(window, Params);
    // And reset the device!
    m_device->Reset(&Params);
    return true;
}

bool DirectX9::BeginContext(Gwk::WindowProvider* window)
{
    m_device->BeginScene();
    m_device->Clear(0, nullptr, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(
                         128, 128,
                         128), 1, 0);
    return true;
}

bool DirectX9::EndContext(Gwk::WindowProvider* window)
{
    m_device->EndScene();
    return true;
}

} // namespace Renderer
} // namespace Gwk
