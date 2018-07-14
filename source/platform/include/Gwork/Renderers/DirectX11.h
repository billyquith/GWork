/*
 *  Gwork
 *  Copyright (c) 2013-2017 Bill Quith
 *  GWEN Renderer provided by Rashawn Clarke - topblast@github
 *  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_DIRECTX11_H
#define GWK_RENDERERS_DIRECTX11_H

#include <Gwork/BaseRender.h>
#include <d3d11.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace Gwk
{
    namespace Renderer
    {

#define GwkDxSafeRelease(var) if(var != nullptr) {var->Release(); var = nullptr;}

        //
        //! Renderer for [DirectX11](https://en.wikipedia.org/wiki/DirectX#DirectX_11).
        //
        class GWK_EXPORT DirectX11 : public Gwk::Renderer::Base
        {
            template<typename T>
            using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

            static const wchar_t BeginCharacter = L' ';    // First Character of Wide Character Table
            static const wchar_t LastCharacter = 0x2FFF;   // Last Character of Wide Character Table
            static const wchar_t NewLineCharacter = L'\n'; // New Line Character
        public:

            DirectX11(ResourcePaths& loader, ID3D11Device* pDevice = nullptr);
            virtual ~DirectX11();

            virtual void Init();

            virtual void Begin();
            virtual void End();
            virtual void Release();

            void SetDrawColor(Gwk::Color color) final;

            void DrawFilledRect(Gwk::Rect rect) final;

            void RenderText(const Gwk::Font& font, Gwk::Point pos, const Gwk::String& text) final;
            Gwk::Point MeasureText(const Gwk::Font& font, const Gwk::String& text) final;

            void StartClip();
            void EndClip();

            void DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect pTargetRect,
                                  float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) final;
            Gwk::Color PixelColor(const Gwk::Texture& texture, unsigned int x, unsigned int y, const Gwk::Color& col_default) final;
            // Resource Loader
            Gwk::Font::Status LoadFont(const Gwk::Font& font) override;
            void FreeFont(const Gwk::Font& font) override;
            bool EnsureFont(const Gwk::Font& font) override;

            Texture::Status LoadTexture(const Gwk::Texture& texture) override;
            void FreeTexture(const Gwk::Texture& texture) override;
            TextureData GetTextureData(const Gwk::Texture& texture) const override;
            bool EnsureTexture(const Gwk::Texture& texture) override;

        protected:

            FLOAT                   width, height;
            FLOAT                   scalex, scaley;
            DWORD                   m_Color;
            bool                    m_Valid;

            //Rendering
            ID3D11Device*           m_pDevice;
            IDXGISwapChain*         m_pSwapChain;
            ID3D11PixelShader*      m_pPixShader;
            ID3D11VertexShader*     m_pVertShader;
            ID3D11DeviceContext*    m_pContext;
            ID3D11BlendState*       m_pBlendState;
            ID3D11InputLayout*      m_pInputLayout;
            ID3D11RasterizerState*  m_pRastState;
            ID3D11ShaderResourceView* m_pCurrentTexture;

            ID3D11BlendState*       m_pUILastBlendState;
            float                   m_LastBlendFactor[4];
            UINT                    m_LastBlendMask;
            UINT                    m_LastStencilRef;
            ID3D11InputLayout*      m_LastInputLayout;
            D3D11_PRIMITIVE_TOPOLOGY m_LastTopology;
            ID3D11Buffer*           m_LastBuffers[8];
            UINT                    m_LastStrides[8];
            UINT                    m_LastOffsets[8];
            ID3D11PixelShader*      m_LastPSShader;
            ID3D11VertexShader*     m_LastVSShader;
            ID3D11GeometryShader*   m_LastGSShader;
            ID3D11ShaderResourceView* m_pLastTexture[8];
            ID3D11DepthStencilState* m_LastDepthState;
            ID3D11RasterizerState*  m_pUILastRasterizerState;
            D3D11_RECT              region;
        protected:// Resourses

            struct DxTextureData : public Gwk::TextureData
            {
                DxTextureData()
                    : m_Texture(nullptr)
                    , m_TextureResource(nullptr)
                {
                }
                DxTextureData(const DxTextureData&) = delete;
                DxTextureData(DxTextureData&& other)
                    : DxTextureData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(readable, other.readable);
                    std::swap(m_Texture, other.m_Texture);
                    std::swap(m_TextureResource, other.m_TextureResource);

                    m_ReadData.swap(other.m_ReadData);
                }

                ~DxTextureData()
                {
                    GwkDxSafeRelease(m_TextureResource);
                    GwkDxSafeRelease(m_Texture);
                }

                ID3D11Texture2D* m_Texture;
                ID3D11ShaderResourceView* m_TextureResource;
                deleted_unique_ptr<unsigned char> m_ReadData;
            };

            struct DxFontData
            {
                DxFontData()
                    : m_Spacing(0.f)
                    , m_Texture(nullptr)
                    , m_TextureResource(nullptr)
                {
                }

                DxFontData(const DxFontData&) = delete;
                DxFontData(DxFontData&& other)
                    : DxFontData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(m_Texture, other.m_Texture);
                    std::swap(m_TextureResource, other.m_TextureResource);
                    std::swap(m_TexCoords, other.m_TexCoords);
                }

                ~DxFontData()
                {
                    GwkDxSafeRelease(m_TextureResource);
                    GwkDxSafeRelease(m_Texture);
                }

                __declspec(align(16)) struct FLOAT4
                {
                    float x, y, z, w;
                    FLOAT4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
                    FLOAT4() : FLOAT4(0, 0, 0, 0) {}
                    FLOAT4& operator= (const FLOAT4& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }
                };

                std::vector<FLOAT4> m_TexCoords;

                float   m_Spacing;

                float width;
                float height;
                ID3D11Texture2D* m_Texture;
                ID3D11ShaderResourceView* m_TextureResource;
            };

            std::unordered_map<Font, DxFontData> m_fonts;
            std::unordered_map<Texture, DxTextureData> m_textures;
            std::pair<const Font, DxFontData>* m_lastFont;
            std::pair<const Texture, DxTextureData>* m_lastTexture;

            void Flush();
            void Present();

            struct VertexFormat
            {
                FLOAT x, y, z;
                DWORD color;
                float u, v;
            };

            template <typename T>
            class VertexBuffer
            {
            protected:
                T* data;
                ID3D11Buffer* m_vbuffer;
                ID3D11DeviceContext* m_pContext;
                DWORD maxVertices;
                DWORD numVertices;
                bool bufferResize;
                bool open;
            public:
                inline VertexBuffer(DWORD _maxVertices = 1)
                    :   bufferResize(true)
                    ,   numVertices(0)
                    ,   maxVertices(_maxVertices)
                    ,   open(false)
                    ,   m_vbuffer(nullptr)
                    ,   m_pContext(nullptr)
                    ,   data(nullptr)
                {}

                inline ~VertexBuffer()
                {
                    if (open)
                        End();
                    GwkDxSafeRelease(m_vbuffer);
                    GwkDxSafeRelease(m_pContext);
                }

                inline DWORD GetMaxVertices() const { return maxVertices; }

                inline DWORD GetNumVertices(void) const { return numVertices; }
                inline void SetNumVertices(DWORD value) { maxVertices = value; }

                inline ID3D11Buffer* & GetBuffer() { return m_vbuffer; }

                inline bool isBufferResizing() const { return (bufferResize || GetMaxVertices() < GetNumVertices()); }


                inline HRESULT Begin(ID3D11Device* pDevice)
                {
                    HRESULT hr;
                    if (bufferResize)
                    {
                        GwkDxSafeRelease(m_vbuffer);

                        D3D11_BUFFER_DESC bufdesc = CD3D11_BUFFER_DESC(maxVertices * sizeof(T), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

                        if (FAILED(hr = pDevice->CreateBuffer(&bufdesc, nullptr, &m_vbuffer)))
                            return hr;

                        bufferResize = false;
                    }

                    pDevice->GetImmediateContext(&m_pContext);

                    D3D11_MAPPED_SUBRESOURCE map;
                    if (FAILED(hr = m_pContext->Map(m_vbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map)))
                        return hr;

                    data = (T*)map.pData;

                    numVertices = 0;

                    open = true;
                    return S_OK;
                }

                inline void Add(const std::vector<T>& vertices)
                {
                    size_t size = vertices.size();
                    if (open && GetMaxVertices() >= GetNumVertices() + size)
                    {
                        memcpy(data, vertices.data(), sizeof(T) * size);
                        data += size;
                    }
                    numVertices += size;
                }

                inline void Add(const T& vertex)
                {
                    if (open && GetMaxVertices() >= GetNumVertices() + 1)
                        *data++ = vertex;
                }

                inline void Add(const T* vertices, size_t len)
                {
                    if (open && GetMaxVertices() >= GetNumVertices() + len)
                    {
                        memcpy(data, vertices, len * sizeof(T));
                        data += len;
                    }
                    numVertices += static_cast<DWORD>(len);
                }

                inline HRESULT End()
                {
                    open = false;

                    m_pContext->Unmap(m_vbuffer, 0);

                    data = nullptr;

                    if (bufferResize = isBufferResizing())
                    {
                        maxVertices = numVertices;
                    }

                    GwkDxSafeRelease(m_pContext);
                    return S_OK;
                }

            };

            VertexBuffer<VertexFormat> m_Buffer;
        };
    }
}

#endif // GWK_RENDERERS_DIRECTX11_H
