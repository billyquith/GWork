/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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
                m_buffer = new Color[m_size.x * m_size.y];
            }
    
            Color& At(int x, int y) { return m_buffer[y * m_size.x + x]; }
            Color& At(Point const& pt) { return At(pt.x, pt.y); }
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

            void SetDrawColor(Gwk::Color color) override;

            void DrawFilledRect(Gwk::Rect rect) override;

            void RenderText(Gwk::Font* font, Gwk::Point pos,
                                    const Gwk::String& text) override;
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;

            void StartClip() override;
            void EndClip() override;

            void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect targetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;
            Gwk::Color PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default) override;

            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false) override;
            // void DrawPixel(int x, int y);

            bool BeginContext(Gwk::WindowProvider* window) override;
            bool EndContext(Gwk::WindowProvider* window) override;
            bool PresentContext(Gwk::WindowProvider* window) override;

        private:
            
            Gwk::Color m_color;
            PixelBuffer *m_pixbuf;
        };


    }
}
#endif // ifndef GWK_RENDERERS_SOFTWARE_H
