/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Billy Quith
*  See license in Gwork.h
 */

#pragma once

#ifndef GWK_RENDERERS_SOFTWARE_H
#define GWK_RENDERERS_SOFTWARE_H

#include <Gwork/BaseRender.h>

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

        //! Default resource loader for software renderer.
        class SoftwareResourceLoader : public ResourceLoader
        {
            ResourcePaths& m_paths;
        public:
            SoftwareResourceLoader(ResourcePaths& paths) : m_paths(paths) {}

            Font::Status LoadFont(Font& font) override;
            void FreeFont(Font& font) override;

            Texture::Status LoadTexture(Texture& texture) override;
            void FreeTexture(Texture& texture) override;
        };

        //
        //! \brief Renders to a buffer without needing external dependencies.
        //!
        //! This can be used for screenshots and testing.
        //
        class GWK_EXPORT Software : public Gwk::Renderer::Base
        {
        public:

            Software(ResourceLoader& loader, PixelBuffer& pbuff);
            virtual ~Software();

            void StartClip() override;
            void EndClip() override;

            void SetDrawColor(Gwk::Color color) override;

            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;
            void RenderText(Gwk::Font* font, Gwk::Point pos,
                            const Gwk::String& text) override;

            void DrawFilledRect(Gwk::Rect rect) override;
            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;
            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawPixel(int x, int y) override;

            Gwk::Color PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default) override;

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
