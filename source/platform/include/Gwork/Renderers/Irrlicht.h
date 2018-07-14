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
#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

namespace Gwk
{
    namespace Renderer
    {
        //
        //  Irrlicht ICacheToTexture
        //
        class IrrlichtCTT;

        //
        //  Irrlicht Renderer
        //
        class GWK_EXPORT Irrlicht : public Gwk::Renderer::Base
        {
            template<typename T>
            using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

            static const wchar_t BeginCharacter = L' ';    // First Character of Wide Character Table
            static const wchar_t LastCharacter = 0x2FFF;   // Last Character of Wide Character Table
            static const wchar_t NewLineCharacter = L'\n'; // New Line Character

        public:
            
            Irrlicht(ResourcePaths& paths, irr::IrrlichtDevice* Device);
            ~Irrlicht();

            void SetDrawColor(Gwk::Color color) override;

            void StartClip() override;
            void EndClip() override;

            ICacheToTexture* GetCTT() override;

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

            struct IRTextureData : public Gwk::TextureData
            {
                IRTextureData()
                {
                }
                IRTextureData(const IRTextureData&) = delete;
                IRTextureData(IRTextureData&& other)
                    : IRTextureData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(readable, other.readable);
                    texture.swap(other.texture);
                }

                ~IRTextureData()
                {

                }

                deleted_unique_ptr<irr::video::ITexture> texture;
                deleted_unique_ptr<unsigned char> m_ReadData;
            };
            /*
            struct IRFontData
            {
                IRFontData()
                {
                }

                IRFontData(const IRFontData&) = delete;
                IRFontData(IRFontData&& other)
                    : IRFontData()
                {
                    tFont.swap(other.tFont);
                }

                ~IRFontData()
                {
                }
                deleted_unique_ptr<irr::gui::IGUIFont> tFont;
            };

            std::unordered_map<Font, IRFontData> m_fonts;*/
            std::unordered_map<Texture, IRTextureData> m_textures;
            std::pair<const Texture, IRTextureData>* m_lastTexture;
            
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
