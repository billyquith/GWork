/*
*  Gwork
*  Copyright (c) 2013-17 Billy Quith
*  See license in Gwork.h
*/

#pragma once

#ifndef GWK_RENDERERS_IRRLICHT_H
#define GWK_RENDERERS_IRRLICHT_H

#include <Gwork/BaseRender.h>
#include <irrlicht.h>

namespace Gwk
{
    namespace Renderer
    {
        //
        //  Irrlicht ICacheToTexture
        //
        class IrrlichtCTT;

        class IrrlichtResourceLoader : public ResourceLoader
        {
        public:
            
            IrrlichtResourceLoader(irr::video::IVideoDriver* VideoDriver, ResourcePaths& paths)
                :   m_driver(VideoDriver)
                ,   m_paths(paths)
            {}

            Font::Status LoadFont(Font& font) override;
            void FreeFont(Font& font) override;

            Texture::Status LoadTexture(Texture& texture) override;
            void FreeTexture(Texture& texture) override;
            
        private:
            
            irr::video::IVideoDriver* m_driver;
            ResourcePaths& m_paths;
        };

        //
        //  Irrlicht Renderer
        //
        class GWK_EXPORT Irrlicht : public Gwk::Renderer::Base
        {
        public:
            
            Irrlicht(irr::IrrlichtDevice* Device, IrrlichtResourceLoader& loader);
            ~Irrlicht();

            void SetDrawColor(Gwk::Color color) override;

            void StartClip() override;
            void EndClip() override;

            Gwk::Color PixelColor(Gwk::Texture* pTexture,
                                  unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default = Gwk::Color(255, 255, 255, 255)
                                 ) override final;

            ICacheToTexture* GetCTT() override;

            void RenderText(Gwk::Font* pFont, Gwk::Point pos, const Gwk::String & text) override;
            Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String & text) override;

            void DrawTexturedRect(Gwk::Texture* pTexture,
                                  Gwk::Rect pTargetRect,
                                  float u1 = 0.0f, float v1 = 0.0f,
                                  float u2 = 1.0f, float v2 = 1.0f) override;

            void DrawFilledRect(Gwk::Rect rect) override;
            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawPixel(int x, int y) override;
            
        private:
            
            IrrlichtCTT* m_CTT;
            irr::video::IVideoDriver* Driver;
            irr::video::SColor DrawColor;
            irr::core::rect<irr::s32> ClipRect;
            irr::gui::IGUIFont* Text;
        };
    }
}
#endif
