/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_SOFTWARE_H
#define GWK_RENDERERS_SOFTWARE_H

#include <Gwork/BaseRender.h>
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

namespace Gwk
{
    namespace Renderer
    {
        //! Simple 2D pixel buffer.
        class PixelBuffer
        {
            Point m_size;
            Color *m_buffer;
        public:
            PixelBuffer() : m_buffer(nullptr) {}
            ~PixelBuffer()
            {
                delete [] m_buffer;
            }
            void Init(Point const& sz)
            {
                m_size = sz;
                m_buffer = new Color[m_size.x * m_size.y];
            }

            Point GetSize() const { return m_size; }

            Color& At(int x, int y) { return m_buffer[y * m_size.x + x]; }
            Color& At(Point const& pt) { return At(pt.x, pt.y); }

            const Color& At(int x, int y) const { return m_buffer[y * m_size.x + x]; }
            const Color& At(Point const& pt) const { return At(pt.x, pt.y); }
        };

        //! \brief Renders to a buffer without needing external dependencies.
        //!
        //! This can be used for screenshots and testing.
        class GWK_EXPORT Software : public Gwk::Renderer::Base
        {
            template<typename T>
            using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

            static const wchar_t BeginCharacter = L' ';    // First Character of Wide Character Table
            static const wchar_t LastCharacter = 0x2FFF;   // Last Character of Wide Character Table
            static const wchar_t NewLineCharacter = L'\n'; // New Line Character

        public:

            Software(ResourcePaths& paths, PixelBuffer& pbuff);
            virtual ~Software();

            void StartClip() override;
            void EndClip() override;

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;
            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawPixel(int x, int y) override;

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

            struct SWTextureData : public Gwk::TextureData
            {
                SWTextureData() {}

                SWTextureData(const SWTextureData&) = delete;
                
                SWTextureData(SWTextureData&& other)
                    : SWTextureData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(readable, other.readable);

                    m_ReadData.swap(other.m_ReadData);
                }

                ~SWTextureData() {}

                Point GetSize() const
                {
                    return Point(static_cast<int>(width), static_cast<int>(height));
                }

                Color& At(int x, int y)
                {
                    return reinterpret_cast<Color*>(m_ReadData.get())[y * static_cast<int>(width) + x];
                }
                Color& At(Point const& pt) { return At(pt.x, pt.y); }

                const Color& At(int x, int y) const
                {
                    return reinterpret_cast<Color*>(m_ReadData.get())[y * static_cast<int>(width) + x];
                }
                const Color& At(Point const& pt) const { return At(pt.x, pt.y); }

                deleted_unique_ptr<unsigned char> m_ReadData;
            };

            struct SWFontData
            {
                SWFontData()
                    : m_Spacing(0.f)
                {}

                SWFontData(const SWFontData&) = delete;
                SWFontData(SWFontData&& other)
                    : SWFontData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(baked_chars, other.baked_chars);

                    m_ReadData.swap(other.m_ReadData);
                }

                ~SWFontData()
                {
                }

                struct stbtt_bakedchar
                {
                    unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
                    float xoff, yoff, xadvance;
                };

                std::vector<stbtt_bakedchar> baked_chars;

                float   m_Spacing;

                float width;
                float height;
                deleted_unique_ptr<unsigned char> m_ReadData;
            };

            std::unordered_map<Font, SWFontData> m_fonts;
            std::unordered_map<Texture, SWTextureData> m_textures;
            std::pair<const Font, SWFontData>* m_lastFont;
            std::pair<const Texture, SWTextureData>* m_lastTexture;
            
        public:

            bool BeginContext(Gwk::WindowProvider* window) override
            {
                return false;
            }

            bool EndContext(Gwk::WindowProvider* window) override
            {
                return false;
            }

            bool PresentContext(Gwk::WindowProvider* window) override
            {
                return false;
            }

        private:

            bool Clip(Rect& rect);
            bool m_isClipping;

            Gwk::Color m_color;
            PixelBuffer *m_pixbuf;
        };


    }
}
#endif // ifndef GWK_RENDERERS_SOFTWARE_H
