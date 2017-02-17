/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_ALLEGRO_H
#define GWK_RENDERERS_ALLEGRO_H

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

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;

            void LoadFont(Gwk::Font* font) override;
            void FreeFont(Gwk::Font* font) override;
            void RenderText(Gwk::Font* font, Gwk::Point pos,
                                    const Gwk::String& text) override;
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;
            void        LoadTexture(Gwk::Texture* texture) override;
            void        FreeTexture(Gwk::Texture* texture) override;
            Gwk::Color  PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default) override;

            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false) override;
            // void DrawPixel(int x, int y);

            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;

            // Cache to texture.
            ICacheToTexture* GetCTT() override;

        protected:

            ALLEGRO_COLOR m_color;
            AllegroCTT *m_ctt;
        };


    }
}
#endif // ifndef GWK_RENDERERS_ALLEGRO_H
