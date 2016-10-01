/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwk::Texture* texture);
            void        FreeTexture(Gwk::Texture* texture);
            Gwk::Color  PixelColour(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default);

            void DrawLinedRect(Gwk::Rect rect);
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false);
            // void DrawPixel(int x, int y);

            bool BeginContext(Gwk::WindowProvider* window);
            bool EndContext(Gwk::WindowProvider* window);
            bool PresentContext(Gwk::WindowProvider* window);

            // Cache to texture.
            ICacheToTexture* GetCTT();

        protected:

            ALLEGRO_COLOR m_color;
            AllegroCTT *m_ctt;
        };


    }
}
#endif // ifndef GWK_RENDERERS_ALLEGRO_H
