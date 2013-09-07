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
#include <SDL2/SDL.h>

namespace Gwen
{
    namespace Renderer
    {
        class SDL2CTT;
        
        class SDL2 : public Gwen::Renderer::Base
        {
        public:

            SDL2(SDL_Window *window);
            ~SDL2();

            virtual void SetDrawColor(Gwen::Color color) override;

            virtual void DrawFilledRect(Gwen::Rect rect) override;

            virtual void LoadFont(Gwen::Font* pFont) override;
            virtual void FreeFont(Gwen::Font* pFont) override;
            virtual void RenderText(Gwen::Font* pFont, Gwen::Point pos,
                                    const Gwen::String& text) override;
            virtual Gwen::Point MeasureText(Gwen::Font* pFont, const Gwen::String& text) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect pTargetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;
            void        LoadTexture(Gwen::Texture* pTexture) override;
            void        FreeTexture(Gwen::Texture* pTexture) override;
            Gwen::Color PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y,
                                    const Gwen::Color& col_default) override;

            void DrawLinedRect(Gwen::Rect rect) override;
//            void DrawShavedCornerRect(Gwen::Rect rect, bool bSlight = false) override;
            // void DrawPixel(int x, int y);

            bool BeginContext(Gwen::WindowProvider* pWindow) override;
            bool EndContext(Gwen::WindowProvider* pWindow) override;
            bool PresentContext(Gwen::WindowProvider* pWindow) override;

            // Cache to texture.
//            ICacheToTexture* GetCTT();

        protected:

            SDL_Window      *m_window;
            SDL_Renderer    *m_renderer;
            SDL_Color        m_color;
//            SDL2CTT *m_ctt;
        };


    }
}
#endif // ifndef GWEN_RENDERERS_ALLEGRO_H
