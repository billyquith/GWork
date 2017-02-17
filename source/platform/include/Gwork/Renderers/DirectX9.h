/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2017 Nick Trout
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

            DirectX9(IDirect3DDevice9* device = nullptr);
            ~DirectX9();

            virtual void Begin();
            virtual void End();
            virtual void Release();

            virtual void SetDrawColor(Gwk::Color color);

            virtual void DrawFilledRect(Gwk::Rect rect);

            virtual void LoadFont(Gwk::Font* font);
            virtual void FreeFont(Gwk::Font* font);
            virtual void RenderText(Gwk::Font* font, Gwk::Point pos,
                                    const Gwk::String& text);
            virtual Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwk::Texture* texture);
            void        FreeTexture(Gwk::Texture* texture);
            Gwk::Color PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default);

        public:

            //
            // Self Initialization
            //

            virtual bool InitializeContext(Gwk::WindowProvider* window);
            virtual bool ShutdownContext(Gwk::WindowProvider* window);
            virtual bool PresentContext(Gwk::WindowProvider* window);
            virtual bool ResizedContext(Gwk::WindowProvider* window, int w, int h);
            virtual bool BeginContext(Gwk::WindowProvider* window);
            virtual bool EndContext(Gwk::WindowProvider* window);

        protected:

            virtual void FillPresentParameters(Gwk::WindowProvider* window,
                                               D3DPRESENT_PARAMETERS& Params);

        protected:

            void*               m_currentTexture;
            IDirect3DDevice9*   m_device;
            IDirect3D9*         m_D3D;
            DWORD m_color;
            Gwk::Font::List m_fontList;
            // Gwk::Texture::List	m_textureList;

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
            VertexFormat m_verts[MaxVerts];
            int m_vertNum;

        };


    }
}
#endif // ifndef GWK_RENDERERS_DIRECTX9_H
