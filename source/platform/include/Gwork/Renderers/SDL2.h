/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_SDL2_H
#define GWK_RENDERERS_SDL2_H

#include <Gwork/BaseRender.h>
#include <Gwork/Platform.h>
#include <SDL.h>


namespace Gwk
{
    namespace Renderer
    {
        class SDL2CTT;

        //! Default resource loader for SDL2.
        class SDL2ResourceLoader : public ResourceLoader
        {
            ResourcePaths& m_paths;
            SDL_Renderer *m_sdlRenderer;
        public:
            SDL2ResourceLoader(ResourcePaths& paths, SDL_Renderer *rdr)
                :   m_paths(paths)
                ,   m_sdlRenderer(rdr)
            {}

            Font::Status LoadFont(Font& font) override;
            void FreeFont(Font& font) override;

            Texture::Status LoadTexture(Texture& texture) override;
            void FreeTexture(Texture& texture) override;
        };

        //
        //! Renderer for [SDL2](https://www.libsdl.org).
        //
        class GWK_EXPORT SDL2 : public Gwk::Renderer::Base
        {
        public:

            SDL2(ResourceLoader& loader, SDL_Window *window);
            virtual ~SDL2();

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;

            void RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text) override;
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;

            Gwk::Color  PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                   const Gwk::Color& col_default) override;

            void DrawLinedRect(Gwk::Rect rect) override;

            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;

        private:

            SDL_Window      *m_window;
            SDL_Renderer    *m_renderer;
            SDL_Color        m_color;
        };

    }
}

#endif // ifndef GWK_RENDERERS_SDL2_H
