/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_ALLEGRO_H
#define GWK_RENDERERS_ALLEGRO_H

#include <Gwork/BaseRender.h>
#include <allegro5/allegro5.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

typedef struct ALLEGRO_FONT ALLEGRO_FONT;
namespace Gwk
{
    namespace Renderer
    {
        class AllegroCTT;
        
        //
        //! Renderer for [Allegro5](http://liballeg.org/).
        //
        class GWK_EXPORT Allegro : public Gwk::Renderer::Base
        {
            template<typename T>
            using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;
        public:

            Allegro(ResourcePaths& paths);
            virtual ~Allegro();

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;


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
        protected:// Resourses

            struct ALTextureData : public Gwk::TextureData
            {
                ALTextureData()
                {
                }
                ALTextureData(const ALTextureData&) = delete;
                ALTextureData(ALTextureData&& other)
                    : ALTextureData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(readable, other.readable);
                    texture.swap(other.texture);
                }

                ~ALTextureData()
                {

                }

                deleted_unique_ptr<ALLEGRO_BITMAP> texture;
            };

            struct ALFontData
            {
                ALFontData()
                {
                }

                ALFontData(const ALFontData&) = delete;
                ALFontData(ALFontData&& other)
                    : ALFontData()
                {
                    font.swap(other.font);
                }

                ~ALFontData()
                {
                }
                deleted_unique_ptr<ALLEGRO_FONT> font;
            };

            std::unordered_map<Font, ALFontData> m_fonts;
            std::unordered_map<Texture, ALTextureData> m_textures;
            std::pair<const Font, ALFontData>* m_lastFont;
            std::pair<const Texture, ALTextureData>* m_lastTexture;
        public:
            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false) override;
            // void DrawPixel(int x, int y);

            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;

            // Cache to texture.
            ICacheToTexture* GetCTT() override;

        private:

            ALLEGRO_COLOR m_color;
            AllegroCTT *m_ctt;
        };


    }
}
#endif // ifndef GWK_RENDERERS_ALLEGRO_H
