/*
 *  Gwork
 *  Copyright (c) 2013-2017 Bill Quith
 *  This is a modified version of the GWEN Renderer provided by Rashawn Clarke <topblast@github>.
 *  See license in Gwork.h
 */

#include <Gwork/Renderers/DirectX11.h>
#include <Gwork/PlatformCommon.h>
#include <Gwork/Utility.h>

#include <DirectXMath.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Gwork\External\stb_image.h>
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include <Gwork/External/stb_truetype.h>

#include <array>

namespace Gwk
{
namespace Renderer
{

using namespace DirectX;    // for XNAMATH vectors etc

#define D3DCOLOR_ARGB(a,r,g,b) \
    ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define D3DCOLOR_COLORVALUE(r,g,b,a) \
    D3DCOLOR_ARGB((DWORD)((a)*255.f), (DWORD)((r)*255.f),(DWORD)((g)*255.f),(DWORD)((b)*255.f))

#pragma region Shaders
static const char pixshader[] =R"(
sampler samp0 : register(s0);
Texture2D tex2D : register(t0);

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 tex : TEXCOORD;
};

float4 main( PS_INPUT input ) : SV_Target
{
    if (input.pos.z != 0.0)
        return tex2D.Sample(samp0, input.tex) * input.col;
    else
        return input.col;
}
)";

static const char vertshader[] = R"(
struct VS_INPUT
{
   float4 pos : POSITION;
   float4 col : COLOR;
   float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 tex : TEXCOORD;
};

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = input.pos;
    output.col = input.col;
    output.tex = input.tex;

    return output;
}
)";

static HRESULT CompileShaderFromMemory(const char* szdata, SIZE_T len, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hr = S_OK;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( _DEBUG )
    // Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows
    // the shaders to be optimized and to run exactly the way they will run in
    // the release configuration of this program.
    dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

    //HRESULT WINAPI D3DCompile(
    //    in      LPCVOID pSrcData,
    //    in      SIZE_T SrcDataSize,
    //    in_opt  LPCSTR pSourceName,
    //    in_opt  const D3D_SHADER_MACRO pDefines,
    //    in_opt  ID3DInclude pInclude,
    //    in_opt  LPCSTR pEntrypoint,
    //    in      LPCSTR pTarget,
    //    in      UINT Flags1,
    //    in      UINT Flags2,
    //    out     ID3DBlob ppCode,
    //    out_opt ID3DBlob ppErrorMsgs
    //);
    ID3DBlob* pErrorBlob = nullptr;
    hr = D3DCompile(szdata, len,        // source/len
                    nullptr,            // source name
                    nullptr,            // defines
                    nullptr,            // include
                    szEntryPoint,       // entry
                    szShaderModel,      // target
                    dwShaderFlags, 0,   // flags
                    ppBlobOut,          // [out] code
                    &pErrorBlob);       // [out] error messages

    if (FAILED(hr))
    {
        if (pErrorBlob)
            pErrorBlob->Release();
        return hr;
    }
    if (pErrorBlob)
        pErrorBlob->Release();

    return S_OK;
}
#pragma endregion

Font::Status DirectX11::LoadFont(const Font& font)
{
    FreeFont(font);
    m_lastFont = nullptr;

    const float realsize = font.size * Scale();
    HDC hDC = CreateCompatibleDC(nullptr);
    DWORD texWidth = 2048, texHeight;
    SetMapMode(hDC, MM_TEXT);

    LOGFONTW fd;
    memset(&fd, 0, sizeof(fd));
    wcscpy_s(fd.lfFaceName, LF_FACESIZE, Utility::Widen(font.facename).c_str());
    fd.lfWidth = 0;

    fd.lfCharSet = DEFAULT_CHARSET;
    fd.lfHeight = realsize * -1.0f;
    fd.lfOutPrecision = OUT_DEFAULT_PRECIS;
    fd.lfItalic = 0;
    fd.lfWeight = font.bold ? FW_BOLD : FW_NORMAL;
#ifdef CLEARTYPE_QUALITY
    fd.lfQuality = realsize < 14 ? DEFAULT_QUALITY : CLEARTYPE_QUALITY;
#else
    fd.lfQuality = PROOF_QUALITY;
#endif
    fd.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

    HFONT hFont = CreateFontIndirectW(&fd);

    if (!hFont)
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", font.facename.c_str());
        return Font::Status::ErrorFileNotFound;
    }

    SelectObject(hDC, hFont);
    SetTextAlign(hDC, TA_TOP);

    wchar_t str[2] = L"x";
    std::vector<DxFontData::FLOAT4> sizes(LastCharacter - BeginCharacter + 1);
    float spacing;
    {
        float x = 0, y = 0;
        SIZE sz;

        GetTextExtentPoint32W(hDC, L" ", 1, &sz);

        spacing = sz.cx;

        for (int c = BeginCharacter; c >= BeginCharacter && c <= LastCharacter; c++)
        {
            str[0] = c;
            GetTextExtentPoint32W(hDC, str, 1, &sz);

            if (x + sz.cx > texWidth)
            {
                x = 0;
                y += sz.cy;
            }

            x = ceilf(x);
            y = ceilf(y);

            sizes[c - BeginCharacter] = DxFontData::FLOAT4(x, y, x + sz.cx, y + sz.cy);

            x += sz.cx + spacing / 2;
        }

        texHeight = y + sz.cy;
    }

    DWORD* pBitmapBits;
    BITMAPINFO bmi;
    ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = texWidth;
    bmi.bmiHeader.biHeight = -static_cast<int>(texHeight);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biBitCount = 32;

    HBITMAP hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)&pBitmapBits, nullptr, 0);

    SelectObject(hDC, hBitmap);

    SetTextColor(hDC, RGB(0xFF, 0xFF, 0xFF));
    SetBkColor(hDC, 0x00000000);

    DxFontData fontData;
    fontData.m_TexCoords.resize(sizes.size());
    int c = 0;
    for each (auto& var in sizes)
    {
        str[0] = BeginCharacter + c;

        float x = var.x; // Ceil before
        float y = var.y; // Ceil before

        ExtTextOutW(hDC, static_cast<int>(x), static_cast<int>(y), ETO_OPAQUE | ETO_CLIPPED, nullptr, str, 1, nullptr);

        fontData.m_TexCoords[c++] = DxFontData::FLOAT4( x / texWidth, y / texHeight, var.z / texWidth, var.w / texHeight );
    }

    fontData.m_Spacing = spacing;
    fontData.width = texWidth;
    fontData.height = texHeight;

    ID3D11Texture2D* buftex;
    D3D11_TEXTURE2D_DESC texdesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM,
        texWidth, texHeight,
        1, 1,
        D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

    if (FAILED(m_pDevice->CreateTexture2D(&texdesc, nullptr, &buftex)))
    {
        DeleteObject(hBitmap);
        DeleteObject(hFont);
        DeleteDC(hDC);

        return Font::Status::ErrorBadData;

    }

    D3D11_MAPPED_SUBRESOURCE texmap;
    if (m_pContext == nullptr || FAILED(m_pContext->Map(buftex, 0, D3D11_MAP_WRITE_DISCARD, 0, &texmap)))
    {
        DeleteObject(hBitmap);
        DeleteObject(hFont);
        DeleteDC(hDC);
        GwkDxSafeRelease(buftex);

        return Font::Status::ErrorBadData;
    }

    //BYTE bAlpha;
    DWORD* pDst32;
    BYTE* pDstRow = (BYTE*)texmap.pData;

    for (DWORD y = 0; y < texHeight; y++)
    {
        pDst32 = (DWORD*)pDstRow;
        for (DWORD x = 0; x < texWidth; x++)
        {
            //bAlpha = BYTE( & 0xFF);
            if (pBitmapBits[texWidth * y + x] > 0)
            {
                DWORD col = pBitmapBits[texWidth * y + x];
                float fcol[] = { ((BYTE*)&col)[3] / 255.f, ((BYTE*)&col)[2] / 255.f, ((BYTE*)&col)[1] / 255.f, ((BYTE*)&col)[0] / 255.f };
                fcol[3] = fcol[2] * 0.30f + fcol[1] * 0.59f + fcol[0] * 0.11f;
                *pDst32++ = D3DCOLOR_COLORVALUE(1.f, 1.f, 1.f, fcol[3]);
            }
            else
                *pDst32++ = 0;
        }
        pDstRow += texmap.RowPitch;
    }

    DeleteObject(hBitmap);
    DeleteObject(hFont);
    DeleteDC(hDC);

    m_pContext->Unmap(buftex, 0);

    if (FAILED(m_pDevice->CreateShaderResourceView(buftex, nullptr, &fontData.m_TextureResource)))
    {
        GwkDxSafeRelease(buftex);
        return Font::Status::ErrorBadData;
    }
    
    m_lastFont = &(*m_fonts.insert(std::make_pair(font, std::move(fontData))).first);

    return Font::Status::Loaded;
}

void DirectX11::FreeFont(const Font& font)
{
    if (m_lastFont != nullptr && m_lastFont->first == font)
        m_lastFont = nullptr;

    m_fonts.erase(font); // calls DxFontData destructor
}

bool DirectX11::EnsureFont(const Font& font)
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

Texture::Status DirectX11::LoadTexture(const Texture& texture)
{
    FreeTexture(texture);
    m_lastTexture = nullptr;

    const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

    DxTextureData texData;

    int width, height, n;
    {
        unsigned char *image = stbi_load(filename.c_str(), &width, &height, &n, 4);
        texData.m_ReadData = deleted_unique_ptr<unsigned char>(image, [](unsigned char* mem) { if (mem) stbi_image_free(mem); });
    }

    if (!texData.m_ReadData)
    {
        Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        return Texture::Status::ErrorFileNotFound;
    }

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA subres;
    subres.pSysMem = texData.m_ReadData.get();
    subres.SysMemPitch = width * n;
    subres.SysMemSlicePitch = 0;


    HRESULT result = m_pDevice->CreateTexture2D(&desc, &subres, &texData.m_Texture);
    if (FAILED(result))
    {
        //"Failed to create Texture2D";
        return Texture::Status::ErrorBadData;
    }

    result = m_pDevice->CreateShaderResourceView(texData.m_Texture, nullptr, &texData.m_TextureResource);
    if (FAILED(result))
    {
        //"Failed to create resource view";
        return Texture::Status::ErrorBadData;
    }
    texData.readable = texture.readable;
    if (!texture.readable)
    {
        texData.m_ReadData.reset();
    }

    texData.width = width;
    texData.height = height;
    m_lastTexture = &(*m_textures.insert(std::make_pair(texture, std::move(texData))).first);
    return Texture::Status::Loaded;
}

void DirectX11::FreeTexture(const Gwk::Texture& texture)
{
    if (m_lastTexture != nullptr && m_lastTexture->first == texture)
        m_lastTexture = nullptr;

    m_textures.erase(texture); // calls DxTextureData destructor
}

TextureData DirectX11::GetTextureData(const Texture& texture) const
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

bool DirectX11::EnsureTexture(const Gwk::Texture& texture)
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

DirectX11::DirectX11(ResourcePaths& paths, ID3D11Device* pDevice)
    :   Base(paths)
    ,   m_pDevice(pDevice)
    ,   m_Buffer(256)
    ,   m_lastFont(nullptr)
    ,   m_lastTexture(nullptr)
{
    m_Valid = false;

    m_pSwapChain = nullptr;
    width = height = 0;

    m_pContext = nullptr;
    m_pRastState = nullptr;
    m_pPixShader = nullptr;
    m_pVertShader = nullptr;
    m_pBlendState = nullptr;
    m_pInputLayout = nullptr;
    m_pCurrentTexture = nullptr;

    if (m_pDevice)
        Init();
}

DirectX11::~DirectX11()
{
    Release();
}

void DirectX11::Init()
{
    m_Valid = false;

    m_pDevice->GetImmediateContext(&m_pContext);

    ID3DBlob* pVSBlob = nullptr;
    ID3DBlob* pPSBlob = nullptr;

    if (FAILED(CompileShaderFromMemory(vertshader, sizeof(vertshader), "main", "vs_4_0", &pVSBlob)))
        return;

    if (FAILED(CompileShaderFromMemory(pixshader, sizeof(pixshader), "main", "ps_4_0", &pPSBlob)))
    {
        GwkDxSafeRelease(pVSBlob);
        GwkDxSafeRelease(pPSBlob);
        return;
    }

    if (FAILED(m_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertShader))
        || FAILED(m_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixShader)))
    {
        GwkDxSafeRelease(pVSBlob);
        GwkDxSafeRelease(pPSBlob);

        Release();
        return;
    }

    GwkDxSafeRelease(pPSBlob);

    std::array<D3D11_INPUT_ELEMENT_DESC, 3> layout =
    {
        D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = static_cast<UINT>(layout.size());

    if (FAILED(m_pDevice->CreateInputLayout(layout.data(), numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout)))
    {
        GwkDxSafeRelease(pVSBlob);

        Release();
        return;
    }

    GwkDxSafeRelease(pVSBlob);

    D3D11_BLEND_DESC blenddesc;
    blenddesc.AlphaToCoverageEnable = FALSE;
    blenddesc.IndependentBlendEnable = FALSE;
    blenddesc.RenderTarget[0].BlendEnable = TRUE;
    blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

    if (FAILED(m_pDevice->CreateBlendState(&blenddesc, &m_pBlendState)))
    {
        Release();
        return;
    }

    D3D11_RASTERIZER_DESC rastdesc = CD3D11_RASTERIZER_DESC(D3D11_FILL_SOLID, D3D11_CULL_NONE, false, 0, 0.f, 0.f, false, true, false, false);

    if (FAILED(m_pDevice->CreateRasterizerState(&rastdesc, &m_pRastState)))
    {
        Release();
        return;
    }

    D3D11_VIEWPORT vp;
    UINT numViewports = 1;
    m_pContext->RSGetViewports(&numViewports, &vp);

    width = vp.Width;
    height = vp.Height;
    scalex = 1 / width * 2.f;
    scaley = 1 / height * 2.f;

    region.left = 0.f;
    region.right = width;
    region.top = 0.f;
    region.bottom = height;
    m_pContext->RSSetScissorRects(1, &region);

    m_Valid = true;
}

void DirectX11::Release()
{
    m_Valid = false;
    
    GwkDxSafeRelease(m_pRastState);
    GwkDxSafeRelease(m_pPixShader);
    GwkDxSafeRelease(m_pBlendState);
    GwkDxSafeRelease(m_pVertShader);
    GwkDxSafeRelease(m_pInputLayout);
    GwkDxSafeRelease(m_pContext);
}

void DirectX11::Begin()
{
    if (FAILED(m_Buffer.Begin(m_pDevice)))
        return;

    if (!m_Valid)
        Init();

    // Save current state
    m_pContext->OMGetBlendState(&m_pUILastBlendState, m_LastBlendFactor, &m_LastBlendMask);
    m_pContext->RSGetState(&m_pUILastRasterizerState);
    m_pContext->OMGetDepthStencilState(&m_LastDepthState, &m_LastStencilRef);
    m_pContext->IAGetInputLayout(&m_LastInputLayout);
    m_pContext->IAGetPrimitiveTopology(&m_LastTopology);
    m_pContext->IAGetVertexBuffers(0, 8, m_LastBuffers, m_LastStrides, m_LastOffsets);
    m_pContext->PSGetShaderResources(0, 8, m_pLastTexture);
    m_pContext->PSGetShader(&m_LastPSShader, nullptr, 0);
    m_pContext->GSGetShader(&m_LastGSShader, nullptr, 0);
    m_pContext->VSGetShader(&m_LastVSShader, nullptr, 0);

    m_pCurrentTexture = m_pLastTexture[0];

    m_pContext->RSSetState(m_pRastState);
    m_pContext->IASetInputLayout(m_pInputLayout);
    m_pContext->OMSetBlendState(m_pBlendState, nullptr, 0xFFFFFFFF);

}

void DirectX11::End()
{
    m_Buffer.End();
    Present();

    m_pContext->OMSetBlendState(m_pUILastBlendState, m_LastBlendFactor, m_LastBlendMask);
    m_pContext->RSSetState(m_pUILastRasterizerState);
    m_pContext->OMSetDepthStencilState(m_LastDepthState, m_LastStencilRef);
    m_pContext->IASetInputLayout(m_LastInputLayout);
    m_pContext->IASetPrimitiveTopology(m_LastTopology);
    m_pContext->IASetVertexBuffers(0, 8, m_LastBuffers, m_LastStrides, m_LastOffsets);
    m_pContext->PSSetShaderResources(0, 8, m_pLastTexture);
    m_pContext->PSSetShader(m_LastPSShader, nullptr, 0);
    m_pContext->GSSetShader(m_LastGSShader, nullptr, 0);
    m_pContext->VSSetShader(m_LastVSShader, nullptr, 0);

    // Remove References
    GwkDxSafeRelease(m_pUILastBlendState);
    GwkDxSafeRelease(m_pUILastRasterizerState);
    GwkDxSafeRelease(m_LastDepthState);
    GwkDxSafeRelease(m_LastInputLayout);
    for each (auto p in m_LastBuffers)
    {
        GwkDxSafeRelease(p);
    }
    for each (auto p in m_pLastTexture)
    {
        GwkDxSafeRelease(p);
    }
    GwkDxSafeRelease(m_LastPSShader);
    GwkDxSafeRelease(m_LastGSShader);
    GwkDxSafeRelease(m_LastVSShader);
}

void DirectX11::Present()
{
    UINT stride = sizeof(VertexFormat);
    UINT offset = 0;

    m_pContext->IASetVertexBuffers(0, 1, &m_Buffer.GetBuffer(), &stride, &offset);

    m_pContext->PSSetShader(m_pPixShader, nullptr, 0);
    m_pContext->PSSetShaderResources(0, 1, &m_pCurrentTexture);

    m_pContext->VSSetShader(m_pVertShader, nullptr, 0);
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pContext->Draw(m_Buffer.GetNumVertices(), 0);
}

void DirectX11::Flush()
{
    if (m_Buffer.GetNumVertices() > 0)
    {
        m_Buffer.End();
        Present();
        m_Buffer.Begin(m_pDevice);
    }
}

void DirectX11::DrawFilledRect(Gwk::Rect rec)
{
    Translate(rec);

    XMFLOAT4A rect(rec.x, rec.y, rec.w, rec.h);

    rect.z = (rect.x + rect.z) * scalex - 1.f;
    rect.w = 1.f - (rect.y + rect.w) * scaley;
    rect.x = rect.x * scalex - 1.f;
    rect.y = 1.f - rect.y * scaley;

    std::array<VertexFormat, 6> verts =
    {
        VertexFormat{ rect.x, rect.w, 0.0f, m_Color, 0.f, 0.f },
        VertexFormat{ rect.x, rect.y, 0.0f, m_Color, 0.f, 0.f },
        VertexFormat{ rect.z, rect.w, 0.0f, m_Color, 0.f, 0.f },
        VertexFormat{ rect.z, rect.y, 0.0f, m_Color, 0.f, 0.f },
        VertexFormat{ rect.z, rect.w, 0.0f, m_Color, 0.f, 0.f },
        VertexFormat{ rect.x, rect.y, 0.0f, m_Color, 0.f, 0.f }
    };

    m_Buffer.Add(verts.data(), verts.size());
}

void DirectX11::SetDrawColor(Gwk::Color color)
{
    m_Color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
}

void DirectX11::RenderText(const Gwk::Font& font, Gwk::Point pos, const Gwk::String & text)
{
    if (!EnsureFont(font))
        return;

    DxFontData& fontData = m_lastFont->second;

    if (m_pCurrentTexture != fontData.m_TextureResource)
    {
        Flush();
        m_pCurrentTexture = fontData.m_TextureResource;
    }

    Translate(pos.x, pos.y);
    XMFLOAT4A loc(pos.x, pos.y, 0, 0);

    float fStartX = loc.x;

    char* text_ptr = const_cast<char*>(text.c_str());
    while (const auto wide_char = Utility::Strings::utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;
        if (wide_char == NewLineCharacter)
        {
            loc.x = fStartX;
            loc.y += (fontData.m_TexCoords[0].w - fontData.m_TexCoords[0].y) * fontData.height;
            continue;
        }
        else if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;

        const auto& texCoord = fontData.m_TexCoords[c];

        loc.z = loc.x + ((texCoord.z - texCoord.x) * fontData.width);
        loc.w = loc.y + ((texCoord.w - texCoord.y) * fontData.height);

        if (c != 0)
        {

            XMFLOAT4A rect(loc);
            rect.z = rect.z * scalex - 1.f;
            rect.w = 1.f - rect.w * scaley;
            rect.x = rect.x * scalex - 1.f;
            rect.y = 1.f - rect.y * scaley;

            std::array<VertexFormat, 6> v;
            v[0] = { rect.x, rect.w, 0.5f, m_Color, texCoord.x, texCoord.w };
            v[1] = { rect.x, rect.y, 0.5f, m_Color, texCoord.x, texCoord.y };
            v[2] = { rect.z, rect.w, 0.5f, m_Color, texCoord.z, texCoord.w };
            v[3] = { rect.z, rect.y, 0.5f, m_Color, texCoord.z, texCoord.y };
            v[4] = v[2];
            v[5] = v[1];

            m_Buffer.Add(v.data(), v.size());
        }
        loc.x += (loc.z - loc.x);
    }
}

Gwk::Point DirectX11::MeasureText(const Gwk::Font& font, const Gwk::String& text)
{
    if (!EnsureFont(font))
        return Gwk::Point(0, 0);

    DxFontData& fontData = m_lastFont->second;

    float fRowWidth = 0.0f;
    float fRowHeight = (fontData.m_TexCoords[0].w - fontData.m_TexCoords[0].y) * fontData.height;
    float fWidth = 0.0f;
    float fHeight = fRowHeight;

    char* text_ptr = const_cast<char*>(text.c_str());
    while (const auto wide_char = Utility::Strings::utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;
        if (wide_char == NewLineCharacter)
        {
            fRowWidth = 0.0f;
            fHeight += fRowHeight;
            continue;
        }
        else if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;

        const auto& texCoord = fontData.m_TexCoords[c];

        const float tx1 = texCoord.x;
        const float tx2 = texCoord.z;

        fRowWidth += (tx2 - tx1)* fontData.width;

        if (fRowWidth > fWidth)
            fWidth = fRowWidth;
    }

    return Gwk::Point(fWidth, fHeight);
}

void DirectX11::StartClip()
{
    Flush();
    const Gwk::Rect & rect = ClipRegion();

    region.left = ceil(((float)rect.x) * Scale());
    region.right = ceil(((float)(rect.x + rect.w)) * Scale());
    region.top = ceil((float)rect.y * Scale());
    region.bottom = ceil(((float)(rect.y + rect.h)) * Scale());

    m_pContext->RSSetScissorRects(1, &region);
}

void DirectX11::EndClip()
{
    Flush();

    region.left = 0.f;
    region.right = width;
    region.top = 0.f;
    region.bottom = height;
    m_pContext->RSSetScissorRects(1, &region);
}

void DirectX11::DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect rec, float u1, float v1, float u2, float v2)
{
    if (!EnsureTexture(texture))
        return DrawMissingImage(rec);

    DxTextureData& texData = m_lastTexture->second;

    ID3D11ShaderResourceView* pImage = texData.m_TextureResource;

    if (!pImage)
        return DrawMissingImage(rec);

    if (m_pCurrentTexture != pImage)
    {
        Flush();
        m_pCurrentTexture = pImage;
    }
    
    Translate(rec);

    XMFLOAT4A rect(rec.x, rec.y, rec.w, rec.h);

    rect.z = (rect.x + rect.z) * scalex - 1.f;
    rect.w = 1.f - (rect.y + rect.w) * scaley;
    rect.x = rect.x * scalex - 1.f;
    rect.y = 1.f - rect.y * scaley;

    std::array<VertexFormat, 6> verts =
    {
        VertexFormat{ rect.x, rect.w, 0.5f, m_Color, u1, v2 },
        VertexFormat{ rect.x, rect.y, 0.5f, m_Color, u1, v1 },
        VertexFormat{ rect.z, rect.w, 0.5f, m_Color, u2, v2 },
        VertexFormat{ rect.z, rect.y, 0.5f, m_Color, u2, v1 },
        VertexFormat{ rect.z, rect.w, 0.5f, m_Color, u2, v2 },
        VertexFormat{ rect.x, rect.y, 0.5f, m_Color, u1, v1 }
    };

    m_Buffer.Add(verts.data(), verts.size());
}

Gwk::Color DirectX11::PixelColor(const Gwk::Texture& texture, unsigned int x, unsigned int y, const Gwk::Color& col_default)
{
    if (!EnsureTexture(texture))
        return col_default;
    
    DxTextureData& texData = m_lastTexture->second;

    if (texData.readable)
    {
        unsigned char *pPixel = texData.m_ReadData.get() + (x + (y * static_cast<unsigned int>(texData.width))) * 4;
        return Gwk::Color(pPixel[0], pPixel[1], pPixel[2], pPixel[3]);
    }

    ID3D11ShaderResourceView* pImage = texData.m_TextureResource;
    if (!pImage)
        return col_default;

    ID3D11Texture2D *t = texData.m_Texture;

    // We have to create a staging texture to copy the texture to, because textures cannot
    // be used as shader resources and have CPU read access at the same time.

    // Furthermore, in DX11 this texture must already exist and have the same exact dimensions as the
    // source texture. So we have to create and destroy it every time, without prior knowledge of the size
    // of the incoming texture. Obviously this is really expensive.
    ID3D11Texture2D* stagingTexture = nullptr;

    DXGI_SAMPLE_DESC sampleDesc = { 1, 0 };

    D3D11_TEXTURE2D_DESC tdesc;
    tdesc.Width = static_cast<unsigned int>(texData.width);
    tdesc.Height = static_cast<unsigned int>(texData.height);
    tdesc.MipLevels = 0;
    tdesc.ArraySize = 1;
    tdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tdesc.SampleDesc = sampleDesc;
    tdesc.Usage = D3D11_USAGE_STAGING;
    tdesc.BindFlags = 0;
    tdesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    tdesc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateTexture2D(&tdesc, nullptr, &stagingTexture)))
    {
        return col_default;
    }

    // Copy the data over to the staging texture
    m_pContext->CopyResource(stagingTexture, t);

    D3D11_MAPPED_SUBRESOURCE msr;
    if (FAILED(m_pContext->Map(stagingTexture, 0, D3D11_MAP_READ, 0, &msr)))
    {
        GwkDxSafeRelease(stagingTexture);
        return col_default;
    }

    texData.m_ReadData = deleted_unique_ptr<unsigned char>(new unsigned char[tdesc.Width * tdesc.Height * 4], [](unsigned char* mem) { if (mem) delete [] (mem); });
    texData.readable = true;

    DWORD* pixels = (DWORD*)msr.pData;
    DWORD color = pixels[msr.RowPitch / 4 * y + x];
    memcpy(texData.m_ReadData.get(), pixels, tdesc.Width * tdesc.Height * 4);
    m_pContext->Unmap(stagingTexture, 0);

    stagingTexture->Release();

    const DWORD a = ((255u << 24u) & color) >> 24u;
    const DWORD r = ((255u << 16u) & color) >> 16u;
    const DWORD g = ((255u << 8u) & color) >> 8u;
    const DWORD b = ((255u) & color);

    return Gwk::Color(r, g, b, a);
}
}
}
