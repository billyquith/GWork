/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
*  See license in Gwork.h
 */

#ifndef GWK_RENDERERS_GDIPLUS_H
#define GWK_RENDERERS_GDIPLUS_H

#include <Gwork/Gwork.h>
#include <Gwork/BaseRender.h>

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

            GDIPlus(HWND hWND = nullptr);
            ~GDIPlus();

            virtual void Begin();
            virtual void End();

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

            int m_width;
            int m_height;

            Gdiplus::Color m_colour;

            HWND m_hWND;
            HDC m_hDC;
            ULONG_PTR m_gdiplusToken;

            Gdiplus::Graphics*      graphics;
        };


        class GDIPlusBuffered : public GDIPlus
        {
        public:

            GDIPlusBuffered(HWND hWND = nullptr);
            ~GDIPlusBuffered();

            virtual void Begin();
            virtual void End();

        private:

            void CreateBackbuffer();
            void DestroyBackbuffer();

            Gdiplus::Bitmap*            m_bitmap;

        };


    }
}
#endif // ifndef GWK_RENDERERS_GDIPLUS_H
