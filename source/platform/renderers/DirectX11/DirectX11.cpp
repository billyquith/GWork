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
static const char pixshader[] = {
    "sampler samp0 : register(s0);\n"
    "Texture2D tex2D : register(t0);\n"
    "struct PS_INPUT\n"
    "{\n"
    "   float4 pos : SV_POSITION;\n"
    "   float4 col : COLOR;\n"
    "   float2 tex : TEXCOORD;\n"
    "};\n"
    "float4 texmain( PS_INPUT input ) : SV_Target\n"
    "{\n"
    "   return tex2D.Sample(samp0, input.tex) * input.col;\n"
    "}\n"
    "float4 main( PS_INPUT input ) : SV_Target\n"
    "{\n"
    "   return input.col;\n"
    "}"
};

static const char vertshader[] = {
    "struct VS_INPUT\n"
    "{\n"
    "   float4 pos : POSITION;\n"
    "   float4 col : COLOR;\n"
    "   float2 tex : TEXCOORD;\n"
    "};\n"
    "struct PS_INPUT\n"
    "{\n"
    "   float4 pos : SV_POSITION;\n"
    "   float4 col : COLOR;\n"
    "   float2 tex : TEXCOORD;\n"
    "};\n"
    "PS_INPUT main( VS_INPUT input )\n"
    "{\n"
    "   PS_INPUT output;\n"
    "   output.pos = input.pos;\n"
    "   output.col = input.col;\n"
    "   output.tex = input.tex;\n"
    "   return output;\n"
    "};\n"
};

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
                    nullptr,               // source name
                    nullptr,               // defines
                    nullptr,               // include
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

const wchar_t BeginCharacter = 0x32;
const wchar_t LastCharacter = 0x7FF;
const wchar_t NewLineCharacter = L'\n';

class FontData
{
public:
    FontData()
        :   m_TexWidth(0)
        ,   m_TexHeight(0)
        ,   m_Spacing(0.f)
        ,   m_Texture(nullptr)
    {
    }

    ~FontData()
    {
        GwkDxSafeRelease(m_Texture);
    }

    std::vector<DirectX::XMFLOAT4> m_TexCoords;

    UINT32  m_TexWidth;
    UINT32  m_TexHeight;
    float   m_Spacing;
    ID3D11ShaderResourceView* m_Texture;
};


Font::Status DirectX11ResourceLoader::LoadFont(Font& font)
{
    DWORD texWidth, texHeight;
    texWidth = 1024;
    HDC hDC = CreateCompatibleDC(nullptr);
    SetMapMode(hDC, MM_TEXT);

    LOGFONTW fd;
    memset(&fd, 0, sizeof(fd));
    wcscpy_s(fd.lfFaceName, LF_FACESIZE, Utility::Widen(font.facename).c_str());
    fd.lfWidth = 0;

    fd.lfCharSet = DEFAULT_CHARSET;
    fd.lfHeight = font.realsize * -1.0f;
    fd.lfOutPrecision = OUT_DEFAULT_PRECIS;
    fd.lfItalic = 0;
    fd.lfWeight = font.bold ? FW_BOLD : FW_NORMAL;
#ifdef CLEARTYPE_QUALITY
    fd.lfQuality = font.realsize < 14 ? DEFAULT_QUALITY : CLEARTYPE_QUALITY;
#else
    fd.lfQuality = PROOF_QUALITY;
#endif
    fd.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

    HFONT hFont = CreateFontIndirectW(&fd);

    if (!hFont)
    {
        Gwk::Log::Write(Log::Level::Error, "Font file not found: %s", font.facename.c_str());
        return font.status = Font::Status::ErrorFileNotFound;
    }

    SelectObject(hDC, hFont);
    SetTextAlign(hDC, TA_TOP);

    wchar_t str[2] = L"x";
    std::vector<XMFLOAT4> sizes;
    float spacing;
    {
        float x = 0, y = 0;
        SIZE sz;

        GetTextExtentPoint32W(hDC, L" ", 1, &sz);

        spacing = sz.cx;

        for (wchar_t c = BeginCharacter; c <= LastCharacter; c++)
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

            sizes.push_back(XMFLOAT4(x, y, x + sz.cx, y + sz.cy));

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

    FontData* fontData = new FontData();

    int c = 0;
    fontData->m_TexCoords.resize(sizes.size());
    for each (auto& var in sizes)
    {
        str[0] = BeginCharacter + c;

        float x = var.x; // Ceil before
        float y = var.y; // Ceil before

        ExtTextOutW(hDC, static_cast<int>(x), static_cast<int>(y), ETO_OPAQUE | ETO_CLIPPED, nullptr, str, 1, nullptr);

        fontData->m_TexCoords[c++] = { x / texWidth, y / texHeight, var.z / texWidth, var.w / texHeight };
    }

    fontData->m_Spacing = spacing;
    fontData->m_TexHeight = texHeight;
    fontData->m_TexWidth = texWidth;

    ID3D11Texture2D* buftex;
    D3D11_TEXTURE2D_DESC texdesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM,
        fontData->m_TexWidth, fontData->m_TexHeight,
        1, 1,
        D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

    if (FAILED(m_pDevice->CreateTexture2D(&texdesc, nullptr, &buftex)))
    {
        delete fontData;
        fontData = nullptr;

        DeleteObject(hBitmap);
        DeleteObject(hFont);
        DeleteDC(hDC);

        return font.status = Font::Status::ErrorBadData;;
    }

    ID3D11DeviceContext* pContext = nullptr;
    m_pDevice->GetImmediateContext(&pContext);

    D3D11_MAPPED_SUBRESOURCE texmap;
    if (pContext == nullptr || FAILED(pContext->Map(buftex, 0, D3D11_MAP_WRITE_DISCARD, 0, &texmap)))
    {
        delete fontData;
        fontData = nullptr;

        DeleteObject(hBitmap);
        DeleteObject(hFont);
        DeleteDC(hDC);
        GwkDxSafeRelease(buftex);

        return font.status = Font::Status::ErrorBadData;;
    }

    //BYTE bAlpha;
    DWORD* pDst32;
    BYTE* pDstRow = (BYTE*)texmap.pData;

    for (UINT32 y = 0; y < fontData->m_TexHeight; y++)
    {
        pDst32 = (DWORD*)pDstRow;
        for (UINT32 x = 0; x < fontData->m_TexWidth; x++)
        {
            //bAlpha = BYTE( & 0xFF);
            if (pBitmapBits[fontData->m_TexWidth * y + x] > 0)
            {
                DWORD col = pBitmapBits[fontData->m_TexWidth * y + x];
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

    pContext->Unmap(buftex, 0);

    if (FAILED(m_pDevice->CreateShaderResourceView(buftex, nullptr, &fontData->m_Texture)))
    {
        delete fontData;
        fontData = nullptr;

        GwkDxSafeRelease(buftex);
        GwkDxSafeRelease(pContext);
        return font.status = Font::Status::ErrorBadData;
    }

    GwkDxSafeRelease(buftex);
    GwkDxSafeRelease(pContext);

    font.data = fontData;
    font.status = Font::Status::Loaded;

    m_FontList.push_back(&font); // Pointer to a reference, who owns this pointer?

    return font.status;
}

void DirectX11ResourceLoader::FreeFont(Font& font)
{
    // FontData* not deleted with Class' destruction

    if (font.IsLoaded())
    {
        if (font.data)
        {
            FontData* pFontData = static_cast<FontData*>(font.data);
            delete pFontData;
            font.data = nullptr;
        }

        m_FontList.remove(&font);  // Pointer to a reference, who owns this pointer?

        font.status = Font::Status::Unloaded;
    }
}

Texture::Status DirectX11ResourceLoader::LoadTexture(Texture& texture)
{
    if (texture.IsLoaded())
        FreeTexture(texture);

    const String filename = m_paths.GetPath(ResourcePaths::Type::Texture, texture.name);

    int width, height, n;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &n, 0);

    ID3D11Texture2D *texture2D = nullptr;
    ID3D11ShaderResourceView* pTex = nullptr;
    if (!data)
    {
        Gwk::Log::Write(Log::Level::Error, "Texture file not found: %s", filename.c_str());
        texture.status = Texture::Status::ErrorFileNotFound;
        goto error;
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
    subres.pSysMem = data;
    subres.SysMemPitch = width * n;
    subres.SysMemSlicePitch = 0;


    HRESULT result = m_pDevice->CreateTexture2D(&desc, &subres, &texture2D);
    if (FAILED(result))
    {
        //"Failed to create Texture2D";
        texture.status = Texture::Status::ErrorBadData;
        goto error;
    }

    result = m_pDevice->CreateShaderResourceView(texture2D, NULL, &pTex);
    if (FAILED(result))
    {
        //"Failed to create resource view";
        texture.status = Texture::Status::ErrorBadData;
        goto error;
    }

    if (texture.readable)
    {
        texture.data = data;
    }
    else
    {
        stbi_image_free(data);
        data = NULL;
    }

    texture.surface = pTex;
    texture.width = width;
    texture.height = height;
    texture.status = Texture::Status::Loaded;
    return texture.status;

error:
    GwkDxSafeRelease(pTex);
    GwkDxSafeRelease(texture2D);
    texture.surface = NULL;
    texture.width = 0;
    texture.height = 0;
    if (data)
        stbi_image_free(data);
    return texture.status;
}

void DirectX11ResourceLoader::FreeTexture(Texture& texture)
{
    if (texture.IsLoaded())
    {
        if (texture.surface)
        {
            ID3D11ShaderResourceView* pImage = static_cast<ID3D11ShaderResourceView*>(texture.surface);
            GwkDxSafeRelease(pImage);
            texture.surface = nullptr;
        }

        if (texture.data != nullptr)
        {
            stbi_image_free(texture.data);
            texture.data = nullptr;
        }

        texture.status = Texture::Status::Unloaded;
    }
}

DirectX11::DirectX11(ResourceLoader& loader, ID3D11Device* pDevice)
    :   Base(loader)
    ,   m_pDevice(pDevice)
    ,   m_Buffer(256)
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
    m_pTexPixShader = nullptr;
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
    ID3DBlob* pTexPSBlob = nullptr;

    if (FAILED(CompileShaderFromMemory(vertshader, sizeof(vertshader), "main", "vs_4_0", &pVSBlob)))
        return;

    if (FAILED(CompileShaderFromMemory(pixshader, sizeof(pixshader), "main", "ps_4_0", &pPSBlob))
        || FAILED(CompileShaderFromMemory(pixshader, sizeof(pixshader), "texmain", "ps_4_0", &pTexPSBlob)))
    {
        GwkDxSafeRelease(pVSBlob);
        GwkDxSafeRelease(pPSBlob);
        GwkDxSafeRelease(pTexPSBlob);
        return;
    }

    if (FAILED(m_pDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertShader))
        || FAILED(m_pDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixShader))
        || FAILED(m_pDevice->CreatePixelShader(pTexPSBlob->GetBufferPointer(), pTexPSBlob->GetBufferSize(), nullptr, &m_pTexPixShader)))
    {
        GwkDxSafeRelease(pVSBlob);
        GwkDxSafeRelease(pPSBlob);
        GwkDxSafeRelease(pTexPSBlob);

        Release();
        return;
    }

    GwkDxSafeRelease(pPSBlob);
    GwkDxSafeRelease(pTexPSBlob);

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = ARRAYSIZE(layout);

    if (FAILED(m_pDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pInputLayout)))
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

    //while (!m_FontList.empty())
    //{
    //    FreeFont(m_FontList.back());
    //}

    GwkDxSafeRelease(m_pRastState);
    GwkDxSafeRelease(m_pPixShader);
    GwkDxSafeRelease(m_pBlendState);
    GwkDxSafeRelease(m_pVertShader);
    GwkDxSafeRelease(m_pInputLayout);
    GwkDxSafeRelease(m_pTexPixShader);
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

    if (m_pCurrentTexture != nullptr)
    {
        m_pContext->PSSetShader(m_pTexPixShader, nullptr, 0);
        m_pContext->PSSetShaderResources(0, 1, &m_pCurrentTexture);
    }
    else
        m_pContext->PSSetShader(m_pPixShader, nullptr, 0);

    m_pContext->VSSetShader(m_pVertShader, nullptr, 0);
    m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_pContext->Draw(m_Buffer.GetNumVertices(), 0);
}

void DirectX11::Flush()
{
    m_Buffer.End();
    Present();
    m_Buffer.Begin(m_pDevice);
}

void DirectX11::DrawFilledRect(Gwk::Rect rec)
{
    if (m_pCurrentTexture != nullptr)
    {
        Flush();
        m_pCurrentTexture = nullptr;
    }

    Translate(rec);

    const float scalex = 1.f / width * 2.f;
    const float scaley = 1.f / height * 2.f;

    XMFLOAT4A rect(rec.x, rec.y, rec.w, rec.h);

    rect.z = (rect.x + rect.z) * scalex - 1.f;
    rect.w = 1.f - (rect.y + rect.w) * scaley;
    rect.x = rect.x * scalex - 1.f;
    rect.y = 1.f - rect.y * scaley;

    VertexFormat verts[6] =
    {
        { rect.x, rect.w, 0.5f, m_Color, 0.f, 0.f },
        { rect.x, rect.y, 0.5f, m_Color, 0.f, 0.f },
        { rect.z, rect.w, 0.5f, m_Color, 0.f, 0.f },
        { rect.z, rect.y, 0.5f, m_Color, 0.f, 0.f },
        { rect.z, rect.w, 0.5f, m_Color, 0.f, 0.f },
        { rect.x, rect.y, 0.5f, m_Color, 0.f, 0.f }
    };

    m_Buffer.Add(verts, ARRAYSIZE(verts));
}

void DirectX11::SetDrawColor(Gwk::Color color)
{
    m_Color = D3DCOLOR_ARGB(color.a, color.r, color.g, color.b);
}


inline wchar_t utf8_to_wchart(char*& in)// Gwk::Utility::Widen too low
{

    unsigned int codepoint;
    while (*in != 0)
    {
        unsigned char ch = static_cast<unsigned char>(*in);
        if (ch <= 0x7f)
            codepoint = ch;
        else if (ch <= 0xbf)
            codepoint = (codepoint << 6) | (ch & 0x3f);
        else if (ch <= 0xdf)
            codepoint = ch & 0x1f;
        else if (ch <= 0xef)
            codepoint = ch & 0x0f;
        else
            codepoint = ch & 0x07;
        ++in;
        if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
        {
            if (sizeof(wchar_t) > 2)
                return static_cast<wchar_t>(codepoint);
            else if (codepoint > 0xffff)
            {
                return static_cast<wchar_t>(0xd800 + (codepoint >> 10));
                return static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff));
            }
            else if (codepoint < 0xd800 || codepoint >= 0xe000)
                return 1, static_cast<wchar_t>(codepoint);
        }
    }
    return 0;
}

void DirectX11::RenderText(Gwk::Font* pFont, Gwk::Point pos, const Gwk::String & text)
{
    Flush();

    FontData* pFontData = (FontData*)pFont->data;

    Translate(pos.x, pos.y);
    XMFLOAT4A loc(pos.x, pos.y, 0, 0);

    FontData* data = (FontData*)pFont->data;

    if (m_pCurrentTexture != data->m_Texture)
    {
        m_pCurrentTexture = data->m_Texture;
    }

    float fStartX = loc.x;

    char* text_ptr = const_cast<char*>(text.c_str());
    while (const auto wide_char = utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;
        if (wide_char == NewLineCharacter)
        {
            loc.x = fStartX;
            loc.y += (data->m_TexCoords[c].w - data->m_TexCoords[c].y) * data->m_TexHeight;
        }
        else if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;

        const auto& texCoord = data->m_TexCoords[c];

        loc.z = loc.x + ((texCoord.z - texCoord.x) * data->m_TexWidth);
        loc.w = loc.y + ((texCoord.w - texCoord.y) * data->m_TexHeight);

        if (c != 0)
        {
            const float scalex = 1.f / (float)width * 2.f;
            const float scaley = 1.f / (float)height * 2.f;

            XMFLOAT4A rect(loc);
            rect.z = rect.z * scalex - 1.f;
            rect.w = 1.f - rect.w * scaley;
            rect.x = rect.x * scalex - 1.f;
            rect.y = 1.f - rect.y * scaley;

            VertexFormat v[6];
            v[0] = { rect.x, rect.w, 0.5f, m_Color, texCoord.x, texCoord.w };
            v[1] = { rect.x, rect.y, 0.5f, m_Color, texCoord.x, texCoord.y };
            v[2] = { rect.z, rect.w, 0.5f, m_Color, texCoord.z, texCoord.w };
            v[3] = { rect.z, rect.y, 0.5f, m_Color, texCoord.z, texCoord.y };
            v[4] = v[2];
            v[5] = v[1];

            m_Buffer.Add(v, ARRAYSIZE(v));
        }
        loc.x += (loc.z - loc.x);
    }

    Flush();
}

Gwk::Point DirectX11::MeasureText(Gwk::Font* pFont, const Gwk::String& text)
{
    // If the font doesn't exist, or the font size should be changed
    //if (!pFont->data || fabs(pFont->realsize - pFont->size * Scale()) > 2)

    if (!EnsureFont(*pFont))
        return Gwk::Point(0, 0);

    FontData* font = (FontData*)pFont->data;

    float fRowWidth = 0.0f;
    float fRowHeight = (font->m_TexCoords[0].w - font->m_TexCoords[0].y) * font->m_TexHeight;
    float fWidth = 0.0f;
    float fHeight = fRowHeight;

    char* text_ptr = const_cast<char*>(text.c_str());
    while (const auto wide_char = utf8_to_wchart(text_ptr))
    {
        const auto c = wide_char - BeginCharacter;
        if (wide_char == NewLineCharacter)
        {
            fRowWidth = 0.0f;
            fHeight += fRowHeight;
        }
        else if (wide_char < BeginCharacter || wide_char > LastCharacter)
            continue;

        const float tx1 = font->m_TexCoords[c].x;
        const float tx2 = font->m_TexCoords[c].z;

        fRowWidth += (tx2 - tx1)* font->m_TexWidth;

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

void DirectX11::DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect rec, float u1, float v1, float u2, float v2)
{
    ID3D11ShaderResourceView* pImage = (ID3D11ShaderResourceView*)pTexture->surface;

    if (!pImage)
        return DrawMissingImage(rec);

    if (m_pCurrentTexture != pImage)
    {
        Flush();
        m_pCurrentTexture = pImage;
    }

    float scalex = 1 / width * 2.f;
    float scaley = 1 / height * 2.f;

    Translate(rec);

    XMFLOAT4A rect(rec.x, rec.y, rec.w, rec.h);

    rect.z = (rect.x + rect.z) * scalex - 1.f;
    rect.w = 1.f - (rect.y + rect.w) * scaley;
    rect.x = rect.x * scalex - 1.f;
    rect.y = 1.f - rect.y * scaley;

    VertexFormat verts[6] =
    {
        { rect.x, rect.w, 0.5f, m_Color, u1, v2 },
        { rect.x, rect.y, 0.5f, m_Color, u1, v1 },
        { rect.z, rect.w, 0.5f, m_Color, u2, v2 },
        { rect.z, rect.y, 0.5f, m_Color, u2, v1 },
        { rect.z, rect.w, 0.5f, m_Color, u2, v2 },
        { rect.x, rect.y, 0.5f, m_Color, u1, v1 }
    };

    m_Buffer.Add(verts, ARRAYSIZE(verts));
}

Gwk::Color DirectX11::PixelColor(Gwk::Texture* pTexture, unsigned int x, unsigned int y, const Gwk::Color& col_default)
{
    if (pTexture->readable)
    {
        unsigned char *pPixel = static_cast<unsigned char*>(pTexture->data) + (x + (y * pTexture->width)) * 4;
        return Gwk::Color(pPixel[0], pPixel[1], pPixel[2], pPixel[3]);
    }

    ID3D11ShaderResourceView* pImage = (ID3D11ShaderResourceView*)pTexture->surface;
    if (!pImage)
        return col_default;

    ID3D11Texture2D *t = nullptr;
    pImage->GetResource(reinterpret_cast<ID3D11Resource**>(&t));

    // We have to create a staging texture to copy the texture to, because textures cannot
    // be used as shader resources and have CPU read access at the same time.

    // Furthermore, in DX11 this texture must already exist and have the same exact dimensions as the
    // source texture. So we have to create and destroy it every time, without prior knowledge of the size
    // of the incoming texture. Obviously this is really expensive.
    ID3D11Texture2D* stagingTexture = nullptr;

    DXGI_SAMPLE_DESC sampleDesc = { 1, 0 };

    D3D11_TEXTURE2D_DESC tdesc;
    tdesc.Width = pTexture->width;
    tdesc.Height = pTexture->height;
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
        GwkDxSafeRelease(t);
        return col_default;
    }

    DWORD* pixels = (DWORD*)msr.pData;
    DWORD color = pixels[msr.RowPitch / sizeof(DWORD) * y + x];
    m_pContext->Unmap(stagingTexture, 0);

    stagingTexture->Release();
    t->Release();

    const DWORD a = ((255u << 24u) & color) >> 24u;
    const DWORD r = ((255u << 16u) & color) >> 16u;
    const DWORD g = ((255u << 8u) & color) >> 8u;
    const DWORD b = ((255u) & color);

    return Gwk::Color(r, g, b, a);
}

#pragma region Experimental
//void DirectX11::FillPresentParameters(Gwk::WindowProvider* pWindow, DXGI_SWAP_CHAIN_DESC & Params)
//{
//    HWND pHWND = (HWND)pWindow->GetWindow();
//    RECT ClientRect;
//    GetClientRect(pHWND, &ClientRect);
//    ZeroMemory(&Params, sizeof(Params));
//
//
//    UINT WIDTH = ClientRect.right - ClientRect.left;
//    UINT HEIGHT = ClientRect.bottom - ClientRect.top;
//
//    Params.BufferCount = 1;
//    Params.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//    Params.OutputWindow = pHWND;
//    Params.SampleDesc.Count = 1;
//    Params.SampleDesc.Quality = 0;
//    Params.Windowed = TRUE;
//
//    Params.BufferDesc.RefreshRate = { 1, 60 };
//    Params.BufferDesc.Width = WIDTH;
//    Params.BufferDesc.Height = HEIGHT;
//    Params.BufferDesc.Format = DXGI_FORMAT_B8G8R8X8_UNORM;
//    Params.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
//}
//
//bool DirectX11::InitializeContext(Gwk::WindowProvider* pWindow)
//{
//    HWND pHWND = (HWND)pWindow->GetWindow();
//    RECT ClientRect;
//    GetClientRect(pHWND, &ClientRect);
//
//    UINT WIDTH = ClientRect.right - ClientRect.left;
//    UINT HEIGHT = ClientRect.bottom - ClientRect.top;
//
//    D3D_DRIVER_TYPE driverTypes[] =
//    {
//        D3D_DRIVER_TYPE_HARDWARE,
//        D3D_DRIVER_TYPE_WARP,
//        D3D_DRIVER_TYPE_REFERENCE,
//    };
//    UINT numDriverTypes = ARRAYSIZE(driverTypes);
//
//    D3D_FEATURE_LEVEL featureLevels[] =
//    {
//        D3D_FEATURE_LEVEL_11_0,
//        D3D_FEATURE_LEVEL_10_1,
//        D3D_FEATURE_LEVEL_10_0,
//        D3D_FEATURE_LEVEL_9_3,
//        D3D_FEATURE_LEVEL_9_2,
//        D3D_FEATURE_LEVEL_9_1
//    };
//    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
//
//
//    DXGI_SWAP_CHAIN_DESC Params;
//    FillPresentParameters(pWindow, Params);
//    HRESULT hr = S_OK;
//    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
//    {
//        D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
//        hr = D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, D3D11_CREATE_DEVICE_SINGLETHREADED, featureLevels, numFeatureLevels,
//            D3D11_SDK_VERSION, &Params, &m_pSwapChain, &m_pDevice, nullptr, &m_pContext);
//        if (SUCCEEDED(hr))
//            break;
//    }
//    if (FAILED(hr))
//        return false;
//
//    ID3D11Texture2D* pBackBuffer = nullptr;
//    hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
//    if (FAILED(hr))
//    {
//        ShutdownContext(pWindow);
//        return false;
//    }
//
//    ID3D11RenderTargetView* pRenderTargetView;
//    hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pRenderTargetView);
//    pBackBuffer->Release();
//    if (FAILED(hr))
//    {
//        ShutdownContext(pWindow);
//        return false;
//    }
//
//    m_pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
//    GwkDxSafeRelease(pRenderTargetView);
//
//    D3D11_VIEWPORT vp;
//    width = vp.Width = (FLOAT)WIDTH;
//    height = vp.Height = (FLOAT)HEIGHT;
//    vp.MinDepth = 0.0f;
//    vp.MaxDepth = 1.0f;
//    vp.TopLeftX = 0;
//    vp.TopLeftY = 0;
//
//    m_pContext->RSSetViewports(1, &vp);
//
//    return true;
//}
//
//bool DirectX11::ShutdownContext(Gwk::WindowProvider* pWindow)
//{
//    ID3D11RenderTargetView* pRenderTargetView;
//    m_pContext->OMGetRenderTargets(1, &pRenderTargetView, 0);
//    GwkDxSafeRelease(pRenderTargetView);
//    GwkDxSafeRelease(m_pSwapChain);
//    GwkDxSafeRelease(m_pContext);
//    GwkDxSafeRelease(m_pDevice);
//    return true;
//}
//
//bool DirectX11::PresentContext(Gwk::WindowProvider* pWindow)
//{
//    m_pSwapChain->Present(1, 0);
//    return true;
//}
//
//bool DirectX11::ResizedContext(Gwk::WindowProvider* pWindow, int w, int h)
//{
//    HWND pHWND = (HWND)pWindow->GetWindow();
//    RECT ClientRect;
//    GetClientRect(pHWND, &ClientRect);
//
//    ID3D11RenderTargetView* pRenderTargetView;
//    m_pContext->OMGetRenderTargets(1, &pRenderTargetView, 0);
//    GwkDxSafeRelease(pRenderTargetView);
//
//    m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
//
//    ID3D11Texture2D* pBuffer;
//    m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
//        (void**)&pBuffer);
//
//    m_pDevice->CreateRenderTargetView(pBuffer, nullptr,
//        &pRenderTargetView);
//    // Perform error handling here!
//    GwkDxSafeRelease(pBuffer);
//
//    m_pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
//    GwkDxSafeRelease(pRenderTargetView);
//
//    UINT WIDTH = ClientRect.right - ClientRect.left;
//    UINT HEIGHT = ClientRect.bottom - ClientRect.top;
//
//    D3D11_VIEWPORT vp;
//    width = vp.Width = (FLOAT)WIDTH;
//    height = vp.Height = (FLOAT)HEIGHT;
//    vp.MinDepth = 0.0f;
//    vp.MaxDepth = 1.0f;
//    vp.TopLeftX = 0;
//    vp.TopLeftY = 0;
//
//    m_pContext->RSSetViewports(1, &vp);
//    return true;
//}
//
//bool DirectX11::BeginContext(Gwk::WindowProvider* pWindow)
//{
//    float ClearColor[4] = { 0.128f, 0.128f, 0.128, 1.0f }; // red,green,blue,alpha
//
//
//    ID3D11RenderTargetView* pRenderTargetView;
//    m_pContext->OMGetRenderTargets(1, &pRenderTargetView, 0);
//    m_pContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
//    pRenderTargetView->Release();
//    return true;
//}
//
//bool DirectX11::EndContext(Gwk::WindowProvider* pWindow)
//{
//    return true;
//}

#pragma endregion
}
}
