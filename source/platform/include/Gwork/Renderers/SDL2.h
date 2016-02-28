/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_ALLEGRO_H
#define GWK_RENDERERS_ALLEGRO_H

#include "Gwork/Gwork.h"
#include "Gwork/BaseRender.h"
#include <SDL.h>


namespace Gwk
{
    namespace Renderer
    {
        class SDL2CTT;
        
        class SDL2 : public Gwk::Renderer::Base
        {
        public:

            SDL2(SDL_Window *window);
            ~SDL2();

            virtual void SetDrawColor(Gwk::Color color) override;

            virtual void DrawFilledRect(Gwk::Rect rect) override;

            virtual void LoadFont(Gwk::Font* pFont) override;
            virtual void FreeFont(Gwk::Font* pFont) override;
            virtual void RenderText(Gwk::Font* pFont, Gwk::Point pos,
                                    const Gwk::String& text) override;
            virtual Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String& text) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;
            void        LoadTexture(Gwk::Texture* pTexture) override;
            void        FreeTexture(Gwk::Texture* pTexture) override;
            Gwk::Color PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default) override;

            void DrawLinedRect(Gwk::Rect rect) override;

            bool BeginContext(Gwk::WindowProvider* pWindow) override;
            bool EndContext(Gwk::WindowProvider* pWindow) override;
            bool PresentContext(Gwk::WindowProvider* pWindow) override;

        protected:

            SDL_Window      *m_window;
            SDL_Renderer    *m_renderer;
            SDL_Color        m_color;
        };


    }
}
#endif // ifndef GWK_RENDERERS_ALLEGRO_H
