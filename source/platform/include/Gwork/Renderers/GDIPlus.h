/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
*  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_GDIPLUS_H
#define GWK_RENDERERS_GDIPLUS_H

#include "Gwork/Gwork.h"
#include "Gwork/BaseRender.h"

#include <gdiplus.h>

/*
 *
 * GDI(plus) is pretty slow for rendering Gwork, because we're
 * re-rendering everything on redraw.
 *
 * Therefore its usage should be as a test - rather than production.
 *
 */

namespace Gwk
{
    namespace Renderer
    {
        class GDIPlus : public Gwk::Renderer::Base
        {
        public:

            GDIPlus(HWND pHWND = NULL);
            ~GDIPlus();

            virtual void Begin();
            virtual void End();

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

            int m_iWidth;
            int m_iHeight;

            Gdiplus::Color m_Colour;

            HWND m_HWND;
            HDC m_hDC;
            ULONG_PTR m_gdiplusToken;

            Gdiplus::Graphics*      graphics;
        };


        class GDIPlusBuffered : public GDIPlus
        {
        public:

            GDIPlusBuffered(HWND pHWND = NULL);
            ~GDIPlusBuffered();

            virtual void Begin();
            virtual void End();

        private:

            void CreateBackbuffer();
            void DestroyBackbuffer();

            Gdiplus::Bitmap*            m_Bitmap;

        };


    }
}
#endif // ifndef GWK_RENDERERS_GDIPLUS_H
