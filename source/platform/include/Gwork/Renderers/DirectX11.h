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

namespace Gwk
{
    namespace Renderer
    {

#define SafeRelease(var) if(var) {var->Release(); var = NULL;}

        class GWK_EXPORT DirectX11 : public Gwk::Renderer::Base
        {
        public:

            DirectX11(ID3D11Device* pDevice = NULL);
            ~DirectX11();

            virtual void Init();

            virtual void Begin();
            virtual void End();
            virtual void Release();

            void SetDrawColor(Gwk::Color color) final;

            void DrawFilledRect(Gwk::Rect rect) final;

            void LoadFont(Gwk::Font* pFont) final;
            void FreeFont(Gwk::Font* pFont) final;
            void RenderText(Gwk::Font* pFont, Gwk::Point pos, const Gwk::String& text) final;
            Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String& text) final;

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect,
                                  float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) final;
            void LoadTexture(Gwk::Texture* pTexture) final;
            void FreeTexture(Gwk::Texture* pTexture) final;
            Gwk::Color PixelColor(Gwk::Texture* pTexture, unsigned int x, unsigned int y, const Gwk::Color& col_default) final;

            //
            // Self Initialization
            //

            //virtual bool InitializeContext(Gwk::WindowProvider* pWindow) final;
            //virtual bool ShutdownContext(Gwk::WindowProvider* pWindow) final;
            //virtual bool PresentContext(Gwk::WindowProvider* pWindow) final;
            //virtual bool ResizedContext(Gwk::WindowProvider* pWindow, int w, int h) final;
            //virtual bool BeginContext(Gwk::WindowProvider* pWindow) final;
            //virtual bool EndContext(Gwk::WindowProvider* pWindow) final;

        protected:

            //virtual void FillPresentParameters(Gwk::WindowProvider* pWindow, DXGI_SWAP_CHAIN_DESC & Params);

            FLOAT               width, height;
            DWORD               m_Color;
            Gwk::Font::List     m_FontList;
            bool m_Valid;

            //Rendering
            ID3D11Device*           m_pDevice;
            IDXGISwapChain*         m_pSwapChain;
            ID3D11PixelShader*      m_pPixShader;
            ID3D11PixelShader*      m_pTexPixShader;
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
            D3D11_RECT region;

            void Flush();
            void Present();
            void AddVert(int x, int y);
            void AddVert(int x, int y, float u, float v);

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
                    SafeRelease(m_vbuffer);
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
                        SafeRelease(m_vbuffer);

                        D3D11_BUFFER_DESC bufdesc = CD3D11_BUFFER_DESC(maxVertices * sizeof(T), D3D11_BIND_VERTEX_BUFFER, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);

                        if (FAILED(hr = pDevice->CreateBuffer(&bufdesc, NULL, &m_vbuffer)))
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
                    auto size = vertices.size();
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

                inline void Add(const T* vertices, int len)
                {
                    if (open && GetMaxVertices() >= GetNumVertices() + len)
                    {
                        memcpy(data, vertices, len * sizeof(T));
                        data += len;
                    }
                    numVertices += len;
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

                    return S_OK;
                }

            };

            VertexBuffer<VertexFormat> m_Buffer;
        };
    }
}

#endif // GWK_RENDERERS_DIRECTX11_H
