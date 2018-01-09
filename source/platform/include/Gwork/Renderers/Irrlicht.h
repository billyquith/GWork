/*
*  Gwork
*  Copyright (c) 2011 Facepunch Studios
*  Copyright (c) 2013-17 Nick Trout
*  See license in Gwork.h
*/

#pragma once

#ifndef GWEN_RENDERERS_IRRLICHT_H
#define GWEN_RENDERERS_IRRLICHT_H

#include "gwork\BaseRender.h"
#include <Irrlicht.h>

namespace Gwk
{
    namespace Renderer
    {
        //
        //  Irrlicht ICacheToTexture
        class IrrlichtCTT;

        class IrrlichtResourceLoader : public ResourceLoader
        {
            ResourcePaths& m_paths;
            irr::video::IVideoDriver* Driver;
        public:
            IrrlichtResourceLoader(ResourcePaths& paths) : m_paths(paths) {}

            Font::Status LoadFont(Font& font) override;
            void FreeFont(Font& font) override;

            Texture::Status LoadTexture(Texture& texture) override;
            void FreeTexture(Texture& texture) override;

            void SetIRRDevice(irr::IrrlichtDevice* Device);
        };

        //
        //  Irrlicht Renderer
        class GWK_EXPORT Irrlicht : public Gwk::Renderer::Base
        {
            IrrlichtCTT* m_CTT;
            irr::video::IVideoDriver* Driver;
            irr::video::SColor DrawColor;
            irr::core::rect<irr::s32> ClipRect;
            irr::gui::IGUIFont* Text;

        public:
            Irrlicht(Gwk::ResourceLoader& loader);
            ~Irrlicht();

            void SetIrrlichtDevice(irr::IrrlichtDevice* Device);


            void SetDrawColor(Gwk::Color color);


            void StartClip();
            void EndClip();

            Gwk::Color PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y, const Gwk::Color & col_default = Gwk::Color(255, 255, 255, 255));

            ICacheToTexture* GetCTT() { return m_CTT; }

            void RenderText(Gwk::Font* pFont, Gwk::Point pos, const Gwk::String & text);
            Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String & text);

            void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect, float u1 = 0.0f, float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
            //void DrawMissingImage(Gwen::Rect pTargetRect);

            void DrawFilledRect(Gwk::Rect rect);
            void DrawLinedRect(Gwk::Rect rect);
            void DrawPixel(int x, int y);

        };
    }
}
#endif
