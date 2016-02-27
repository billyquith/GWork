/*
 *  GWEN
 *  Copyright (c) 2011 Facepunch Studios
 *  See license in Gwen.h
 */

#pragma once

#ifndef GWEN_RENDERERS_ALLEGRO_H
#define GWEN_RENDERERS_ALLEGRO_H

#include "Gwen/Gwen.h"
#include "Gwen/BaseRender.h"
#include <allegro5/allegro5.h>

namespace Gwen
{
    namespace Renderer
    {
        class AllegroCTT;
        
        class Allegro : public Gwen::Renderer::Base
        {
        public:

            Allegro();
            ~Allegro();

            virtual void SetDrawColor(Gwen::Color color);

            virtual void DrawFilledRect(Gwen::Rect rect);

            virtual void LoadFont(Gwen::Font* pFont);
            virtual void FreeFont(Gwen::Font* pFont);
            virtual void RenderText(Gwen::Font* pFont, Gwen::Point pos,
                                    const Gwen::String& text);
            virtual Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::String& text);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect pTargetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwen::Texture* pTexture);
            void        FreeTexture(Gwen::Texture* pTexture);
            Gwen::Color PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y,
                                    const Gwen::Color& col_default);

            void DrawLinedRect(Gwen::Rect rect);
            void DrawShavedCornerRect(Gwen::Rect rect, bool bSlight = false);
            // void DrawPixel(int x, int y);

            bool BeginContext(Gwen::WindowProvider* pWindow);
            bool EndContext(Gwen::WindowProvider* pWindow);
            bool PresentContext(Gwen::WindowProvider* pWindow);

            // Cache to texture.
            ICacheToTexture* GetCTT();

        protected:

            ALLEGRO_COLOR m_Color;
            AllegroCTT *m_ctt;
        };


    }
}
#endif // ifndef GWEN_RENDERERS_ALLEGRO_H
