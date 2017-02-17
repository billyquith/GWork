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

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;

            void LoadFont(Gwk::Font* font) override;
            void FreeFont(Gwk::Font* font) override;
            virtual void RenderText(Gwk::Font* font, Gwk::Point pos,
                                    const Gwk::String& text) override;
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;
            void        LoadTexture(Gwk::Texture* texture) override;
            void        FreeTexture(Gwk::Texture* texture) override;
            Gwk::Color PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default) override;

            void DrawLinedRect(Gwk::Rect rect) override;

            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;

        protected:

            SDL_Window      *m_window;
            SDL_Renderer    *m_renderer;
            SDL_Color        m_color;
        };


    }
}
#endif // ifndef GWK_RENDERERS_ALLEGRO_H
