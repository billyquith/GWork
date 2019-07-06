/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_SDL2_H
#define GWK_RENDERERS_SDL2_H

#include <Gwork/BaseRender.h>
#include <Gwork/Platform.h>
#include <SDL.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>


typedef struct _TTF_Font TTF_Font;

namespace Gwk
{
    namespace Renderer
    {
        class SDL2CTT;

        //
        //! Renderer for [SDL2](https://www.libsdl.org).
        //
        class GWK_EXPORT SDL2 : public Gwk::Renderer::Base
        {
            template<typename T>
            using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

            static const wchar_t BeginCharacter = L' ';    // First Character of Wide Character Table
            static const wchar_t LastCharacter = 0x2FFF;   // Last Character of Wide Character Table
            static const wchar_t NewLineCharacter = L'\n'; // New Line Character

        public:

            SDL2(ResourcePaths& paths, SDL_Window *window);
            virtual ~SDL2();

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;

            void DrawLinedRect(Gwk::Rect rect) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(const Gwk::Texture& texture, Gwk::Rect targetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f) override;

            Gwk::Color PixelColor(const Gwk::Texture& texture,
                                  unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default) override;

            void RenderText(const Gwk::Font& font,
                            Gwk::Point pos,
                            const Gwk::String& text) override;

            Gwk::Point MeasureText(const Gwk::Font& font, const Gwk::String& text) override;

            // Resource Loader
            Gwk::Font::Status LoadFont(const Gwk::Font& font) override;
            void FreeFont(const Gwk::Font& font) override;
            bool EnsureFont(const Gwk::Font& font) override;

            Texture::Status LoadTexture(const Gwk::Texture& texture) override;
            void FreeTexture(const Gwk::Texture& texture) override;
            TextureData GetTextureData(const Gwk::Texture& texture) const override;
            bool EnsureTexture(const Gwk::Texture& texture) override;

            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;

        private:

            struct SDL2TextureData : public Gwk::TextureData
            {
                SDL2TextureData() = default;

                SDL2TextureData(const SDL2TextureData&) = delete;
                
                SDL2TextureData(SDL2TextureData&& other)
                    : SDL2TextureData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(readable, other.readable);
                    texture.swap(other.texture);
                    surface.swap(other.surface);
                }

                ~SDL2TextureData()
                {}

                deleted_unique_ptr<SDL_Texture> texture;
                deleted_unique_ptr<SDL_Surface> surface;
            };

            struct SDL2FontData
            {
                SDL2FontData() = default;

                SDL2FontData(const SDL2FontData&) = delete;
                
                SDL2FontData(SDL2FontData&& other)
                    : SDL2FontData()
                {
                    tFont.swap(other.tFont);
                }

                ~SDL2FontData() {}

                deleted_unique_ptr<TTF_Font> tFont;
            };

            std::unordered_map<Font, SDL2FontData> m_fonts;
            std::unordered_map<Texture, SDL2TextureData> m_textures;
            std::pair<const Font, SDL2FontData>* m_lastFont;
            std::pair<const Texture, SDL2TextureData>* m_lastTexture;
            
        private:

            SDL_Window      *m_window;
            SDL_Renderer    *m_renderer;
            SDL_Color        m_color;
        };

    }
}

#endif // ifndef GWK_RENDERERS_SDL2_H
