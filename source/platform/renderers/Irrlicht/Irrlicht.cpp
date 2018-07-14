/*
*  Gwork
*  Copyright (c) 2013-2017 Billy Quith
*  See license in Gwork.h
*/

#include <Gwork/Renderers/Irrlicht.h>

#pragma comment(lib, "Irrlicht.lib")

#include <iostream>
#include <map>

namespace Gwk {
    namespace Renderer {

        // Resource Loader
        Font::Status Irrlicht::LoadFont(const Gwk::Font& font)
        {
            return Font::Status::Loaded;
        }

        void Irrlicht::FreeFont(const Gwk::Font& font)
        {
        }

        bool Irrlicht::EnsureFont(const Gwk::Font& font)
        {
            return LoadFont(font) == Font::Status::Loaded;
        }

        Texture::Status Irrlicht::LoadTexture(const Gwk::Texture& texture)
        {
            FreeTexture(texture);
            m_lastTexture = nullptr;

            const String filename = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

            irr::video::ITexture* NewTex = Driver->getTexture(filename.c_str());
            if (!NewTex)
            {
                return Texture::Status::ErrorFileNotFound;
            }
            else
            {
                IRTextureData texData;

                const irr::core::dimension2d<irr::u32> TexSize = NewTex->getSize();
                texData.texture = deleted_unique_ptr<irr::video::ITexture>(NewTex, [this](irr::video::ITexture* mem) { if (mem) Driver->removeTexture(mem); });
                texData.readable = false;
                texData.width = TexSize.Width;
                texData.height = TexSize.Height;
                m_lastTexture = &(*m_textures.insert(std::make_pair(texture, std::move(texData))).first);
                return Texture::Status::Loaded;
            }
        }

        void Irrlicht::FreeTexture(const Gwk::Texture& texture)
        {
            if (m_lastTexture != nullptr && m_lastTexture->first == texture)
                m_lastTexture = nullptr;

            m_textures.erase(texture); // calls IRTextureData destructor
        }

        TextureData Irrlicht::GetTextureData(const Texture& texture) const
        {
            if (m_lastTexture != nullptr && m_lastTexture->first == texture)
                return m_lastTexture->second;

            auto it = m_textures.find(texture);
            if (it != m_textures.cend())
            {
                return it->second;
            }
            // Texture not loaded :(
            return TextureData();
        }

        bool Irrlicht::EnsureTexture(const Gwk::Texture& texture)
        {
            if (m_lastTexture != nullptr)
            {
                if (m_lastTexture->first == texture)
                    return true;
            }

            // Was it loaded before?
            auto it = m_textures.find(texture);
            if (it != m_textures.end())
            {
                m_lastTexture = &(*it);
                return true;
            }

            // No, try load to it

            // LoadTexture sets m_lastTexture, if exist
            return LoadTexture(texture) == Texture::Status::Loaded;
        }

        //
        //  Irrlicht ICacheToTexture
        //
        class IrrlichtCTT : public Gwk::Renderer::ICacheToTexture
        {
        private:
            Gwk::Renderer::Base *m_renderer;

            irr::video::IVideoDriver* m_Driver;
            std::map<CacheHandle, irr::video::ITexture*> m_TextureCache;

        public:
            IrrlichtCTT(irr::video::IVideoDriver* VideoDriver) : m_Driver(VideoDriver) {}

            ~IrrlichtCTT()
            {
                //  Cleanup all cached textures
                for (auto it : m_TextureCache)
                {
                    m_Driver->removeTexture(it.second);
                }
            }

            void SetRenderer(Gwk::Renderer::Base* renderer) { m_renderer = renderer; }

            void Initialize() {}
            void ShutDown() {}

            //
            //  Create this controls cached texture if it doesn't already exist
            void CreateControlCacheTexture(CacheHandle control, const Point& size)
            {
                if (m_TextureCache.find(control) == m_TextureCache.end())
                {
                    irr::video::ITexture* RTT =
                        m_Driver->addRenderTargetTexture(
                            irr::core::dimension2d<irr::u32>(size.x, size.y));
                    m_TextureCache.insert(std::pair<CacheHandle, irr::video::ITexture*>(control, RTT));
                }
            }

            //  Setup Irrlicht to cache this control to a texture
            void SetupCacheTexture(CacheHandle control)
            {
                auto it = m_TextureCache.find(control);
                if (it != m_TextureCache.end())
                {
                    m_Driver->setRenderTarget((*it).second);
                }
            }

            //  Revert Irrlicht's render target
            void FinishCacheTexture(CacheHandle control)
            {
                m_Driver->setRenderTarget(NULL);
            }

            //  Draw this controls cached texture
            void DrawCachedControlTexture(CacheHandle control)
            {
                auto it = m_TextureCache.find(control);
                if (it != m_TextureCache.end())
                {
                    const Gwk::Point &pos = m_renderer->GetRenderOffset();
                    m_Driver->draw2DImage((*it).second,
                        irr::core::vector2di(pos.x, pos.y),
                        irr::core::rect<irr::s32>(irr::core::dimension2d<irr::s32>(0, 0),
                        (*it).second->getSize()),
                        NULL,
                        irr::video::SColor(255, 255, 255, 255),
                        true);
                }
            }

            void UpdateControlCacheTexture(CacheHandle control) {}
        };


        //
        //  Irrlicht Renderer
        //
        Irrlicht::Irrlicht(ResourcePaths& paths, irr::IrrlichtDevice* Device)
            : Base(paths)
            , m_CTT(new IrrlichtCTT(Device->getVideoDriver()))
            , Driver(Device->getVideoDriver())
            , m_lastTexture(nullptr)
        {
            m_CTT->SetRenderer(this);
            m_CTT->Initialize();
            DrawColor.set(255, 255, 255, 255);
            ClipRect = irr::core::rect<irr::s32>();
            Text = Device->getGUIEnvironment()->getBuiltInFont();
        }

        Irrlicht::~Irrlicht()
        {
            delete m_CTT;
        }

        void Irrlicht::SetDrawColor(Gwk::Color color)
        {
            DrawColor.set((irr::u32)color.a, (irr::u32)color.r, (irr::u32)color.g, (irr::u32)color.b);
        }

        void Irrlicht::StartClip()
        {
            const Gwk::Rect rect = ClipRegion();
            ClipRect.UpperLeftCorner = irr::core::vector2di(rect.x, rect.y);
            ClipRect.LowerRightCorner = irr::core::vector2di(rect.x + rect.w, rect.y + rect.h);
        }

        void Irrlicht::EndClip()
        {
            ClipRect.UpperLeftCorner = irr::core::vector2di(0, 0);
            ClipRect.LowerRightCorner = irr::core::vector2di(0, 0);
        }


        Gwk::Color Irrlicht::PixelColor(const Gwk::Texture& texture,
            unsigned int x, unsigned int y, const Gwk::Color& col_default)
        {

            if (!EnsureTexture(texture))
                return col_default;

            IRTextureData& texData = m_lastTexture->second;

            irr::video::ITexture* Texture = texData.texture.get();

            const irr::u32 pitch = Texture->getPitch();
            const irr::video::ECOLOR_FORMAT format = Texture->getColorFormat();
            const irr::u32 bytes = irr::video::IImage::getBitsPerPixelFromFormat(format) / 8;

            if (!texData.readable)
            {

                unsigned char* buffer = (unsigned char*)Texture->lock();
                if (buffer)
                {
                    texData.m_ReadData = deleted_unique_ptr<unsigned char>(
                        new unsigned char[texData.width * texData.height * bytes],
                        [](unsigned char* mem) { if (mem) delete[](mem); });
                    memcpy(texData.m_ReadData.get(), buffer, texData.width * texData.height * bytes);
                    texData.readable = true;
                    const irr::video::SColor pixelColor = irr::video::SColor(*(unsigned int*)(buffer + (y * pitch) + (x*bytes)));
                    Texture->unlock();
                    return Gwk::Color(pixelColor.getRed(),
                        pixelColor.getGreen(),
                        pixelColor.getBlue(),
                        pixelColor.getAlpha());
                }
            }
            else
            {

                const irr::video::SColor pixelColor = irr::video::SColor(*(unsigned int*)(texData.m_ReadData.get() + (y * pitch) + (x*bytes)));
                return Gwk::Color(pixelColor.getRed(),
                    pixelColor.getGreen(),
                    pixelColor.getBlue(),
                    pixelColor.getAlpha());
            }
            return col_default;
        }

        void Irrlicht::RenderText(const Gwk::Font& font, Gwk::Point pos, const Gwk::String & text)
        {
            Translate(pos.x, pos.y);
            Text->draw(text.c_str(),
                irr::core::rect<irr::s32>(pos.x, pos.y, pos.x, pos.y),
                DrawColor,
                false,
                false,
                &ClipRect);
        }

        Gwk::Point Irrlicht::MeasureText(const Gwk::Font& font, const Gwk::String & text)
        {
            // Get size of string from Irrlicht (hopefully correct for TTF stuff)
            const std::wstring wText(text.begin(), text.end());
            const irr::core::dimension2d<irr::u32> irrDimension = Text->getDimension(wText.c_str());

            return Gwk::Point(irrDimension.Width, irrDimension.Height);;
        }

        void Irrlicht::DrawTexturedRect(const Gwk::Texture& texture,
            Gwk::Rect pTargetRect,
            float u1, float v1, float u2, float v2)
        {
            if (!EnsureTexture(texture))
                return DrawMissingImage(pTargetRect);

            Translate(pTargetRect);

            IRTextureData& texData = m_lastTexture->second;

            const unsigned int w = texData.width;
            const unsigned int h = texData.height;

            Driver->draw2DImage(texData.texture.get(),
                irr::core::rect<irr::s32>(pTargetRect.x,
                    pTargetRect.y,
                    pTargetRect.x + pTargetRect.w,
                    pTargetRect.y + pTargetRect.h),
                irr::core::rect<irr::s32>(u1*w, v1*h, u2*w, v2*h),
                &ClipRect,
                0,
                true);
        }

        void Irrlicht::DrawFilledRect(Gwk::Rect rect)
        {
            Translate(rect);
            Driver->draw2DRectangle(DrawColor,
                irr::core::rect<irr::s32>(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h), &ClipRect);
        }

        void Irrlicht::DrawLinedRect(Gwk::Rect rect)
        {
            Translate(rect);
            Driver->draw2DRectangleOutline(
                irr::core::rect<irr::s32>(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h),
                DrawColor);
        }

        void Irrlicht::DrawPixel(int x, int y)
        {
            Translate(x, y);
            Driver->drawPixel(x, y, DrawColor);
        }

        ICacheToTexture* Irrlicht::GetCTT() { return m_CTT; }

    }
}
