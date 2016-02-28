/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2015-2016 Billy Quith
 *  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_DIRECTX9_H
#define GWK_RENDERERS_DIRECTX9_H

#include <Gwork/Gwork.h>
#include <Gwork/BaseRender.h>

#include <D3D9.h>
#include <D3DX9Core.h>

#pragma comment( lib, "D3D9.lib" )
#pragma comment( lib, "D3Dx9.lib" )

namespace Gwk
{
    namespace Renderer
    {
        class GWK_EXPORT DirectX9 : public Gwk::Renderer::Base
        {
        public:

            DirectX9(IDirect3DDevice9* pDevice = NULL);
            ~DirectX9();

            virtual void Begin();
            virtual void End();
            virtual void Release();

            virtual void SetDrawColor(Gwk::Color color);

            virtual void DrawFilledRect(Gwk::Rect rect);

            virtual void LoadFont(Gwk::Font* pFont);
            virtual void FreeFont(Gwk::Font* pFont);
            virtual void RenderText(Gwk::Font* pFont, Gwk::Point pos,
                                    const Gwk::String& text);
            virtual Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String& text);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwk::Texture* pTexture);
            void        FreeTexture(Gwk::Texture* pTexture);
            Gwk::Color PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default);

        public:

            //
            // Self Initialization
            //

            virtual bool InitializeContext(Gwk::WindowProvider* pWindow);
            virtual bool ShutdownContext(Gwk::WindowProvider* pWindow);
            virtual bool PresentContext(Gwk::WindowProvider* pWindow);
            virtual bool ResizedContext(Gwk::WindowProvider* pWindow, int w, int h);
            virtual bool BeginContext(Gwk::WindowProvider* pWindow);
            virtual bool EndContext(Gwk::WindowProvider* pWindow);

        protected:

            virtual void FillPresentParameters(Gwk::WindowProvider* pWindow,
                                               D3DPRESENT_PARAMETERS& Params);

        protected:

            void*               m_pCurrentTexture;
            IDirect3DDevice9*   m_pDevice;
            IDirect3D9*         m_pD3D;
            DWORD m_Color;
            Gwk::Font::List m_FontList;
            // Gwk::Texture::List	m_TextureList;

            void Flush();
            void AddVert(int x, int y);
            void AddVert(int x, int y, float u, float v);

        protected:

            struct VertexFormat
            {
                FLOAT x, y, z, rhw;
                DWORD color;
                float u, v;
            };


            static const int MaxVerts = 1024;
            VertexFormat m_pVerts[MaxVerts];
            int m_iVertNum;

        };


    }
}
#endif // ifndef GWK_RENDERERS_DIRECTX9_H
