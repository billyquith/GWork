/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
*  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_DIRECT2D_H
#define GWK_RENDERERS_DIRECT2D_H

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "Gwork/Gwork.h"
#include "Gwork/BaseRender.h"
#include "Gwork/Texture.h"

namespace Gwk
{
    namespace Renderer
    {
        class GWK_EXPORT Direct2D : public Gwk::Renderer::Base
        {
        public:

            Direct2D();
            Direct2D(ID2D1RenderTarget* pDevice, IDWriteFactory* pDWriteFactory,
                     IWICImagingFactory* pWICFactory);
            ~Direct2D();

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

            virtual void DeviceLost();
            virtual void DeviceAcquired(ID2D1RenderTarget* pRT);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwk::Texture* pTexture);
            void        FreeTexture(Gwk::Texture* pTexture);
            Gwk::Color PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default);

            void DrawLinedRect(Gwk::Rect rect);
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false);

        public:

            //
            // Self Initialization
            //

            bool InitializeContext(Gwk::WindowProvider* pWindow);
            bool ShutdownContext(Gwk::WindowProvider* pWindow);
            bool PresentContext(Gwk::WindowProvider* pWindow);
            bool ResizedContext(Gwk::WindowProvider* pWindow, int w, int h);
            bool BeginContext(Gwk::WindowProvider* pWindow);
            bool EndContext(Gwk::WindowProvider* pWindow);

        private:

            bool InternalCreateDeviceResources();
            void InternalReleaseDeviceResources();

            ID2D1Factory*   m_pD2DFactory;
            HWND m_pHWND;

        private:

            bool InternalLoadTexture(Gwk::Texture* pTexture);
            bool InternalLoadFont(Gwk::Font* pFont);

            void InternalFreeFont(Gwk::Font* pFont, bool bRemove = true);
            void InternalFreeTexture(Gwk::Texture* pTexture, bool bRemove = true);

        private:

            IDWriteFactory*     m_pDWriteFactory;
            IWICImagingFactory* m_pWICFactory;
            ID2D1RenderTarget*  m_pRT;

            ID2D1SolidColorBrush* m_pSolidColorBrush;

            D2D1::ColorF m_Color;

            Gwk::Texture::List m_TextureList;
            Gwk::Font::List m_FontList;

        };


    }
}
#endif // ifndef GWK_RENDERERS_DIRECT2D_H
