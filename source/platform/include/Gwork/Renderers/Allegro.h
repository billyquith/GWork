/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_ALLEGRO_H
#define GWK_RENDERERS_ALLEGRO_H

#include <Gwork/Gwork.h>
#include <Gwork/BaseRender.h>
#include <allegro5/allegro5.h>

namespace Gwk
{
    namespace Renderer
    {
        class AllegroCTT;
        
        class Allegro : public Gwk::Renderer::Base
        {
        public:

            Allegro();
            ~Allegro();

            virtual void SetDrawColor(Gwk::Color color);

            virtual void DrawFilledRect(Gwk::Rect rect);

            virtual void LoadFont(Gwk::Font* pFont);
            virtual void FreeFont(Gwk::Font* pFont);
            virtual void RenderText(Gwk::Font* pFont, Gwk::Point pos,
                                    const Gwk::String& text);
            virtual Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String& text);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwk::Texture* pTexture);
            void        FreeTexture(Gwk::Texture* pTexture);
            Gwk::Color PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default);

            void DrawLinedRect(Gwk::Rect rect);
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false);
            // void DrawPixel(int x, int y);

            bool BeginContext(Gwk::WindowProvider* pWindow);
            bool EndContext(Gwk::WindowProvider* pWindow);
            bool PresentContext(Gwk::WindowProvider* pWindow);

            // Cache to texture.
            ICacheToTexture* GetCTT();

        protected:

            ALLEGRO_COLOR m_Color;
            AllegroCTT *m_ctt;
        };


    }
}
#endif // ifndef GWK_RENDERERS_ALLEGRO_H
