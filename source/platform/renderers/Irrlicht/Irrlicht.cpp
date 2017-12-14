#include "gwork/Renderers/Irrlicht.h"

#pragma comment(lib, "Irrlicht.lib")

#include <iostream>
#include <map>

namespace Gwk
{
    namespace Renderer
    {
        //
        //  Resource Loader
        void IrrlichtResourceLoader::SetIRRDevice(irr::IrrlichtDevice* Device)
        {
            Driver = Device->getVideoDriver();
        }

        Font::Status IrrlichtResourceLoader::LoadFont(Font& font)
        {
            printf("Load Font\n");
            return Font::Status::Loaded;
        }

        void IrrlichtResourceLoader::FreeFont(Gwk::Font& font)
        {
            printf("Free Font\n");
        }

        Texture::Status IrrlichtResourceLoader::LoadTexture(Texture& texture)
        {
            printf("Load Texture\n");
            if (texture.IsLoaded())
                FreeTexture(texture);

            const String filename = m_paths.GetPath(ResourcePaths::Type::Texture, texture.name);

            irr::video::ITexture* NewTex = Driver->getTexture(filename.c_str());
            if (!NewTex) {
                texture.status = Texture::Status::ErrorFileNotFound;
            }
            else {
                const irr::core::dimension2d<irr::u32> TexSize = NewTex->getSize();
                texture.width = TexSize.Width;
                texture.height = TexSize.Height;
                texture.data = NewTex;
                texture.status = Texture::Status::Loaded;
            }

            return texture.status;
        }

        void IrrlichtResourceLoader::FreeTexture(Texture& texture)
        {
            printf("Free Texture\n");
            if (texture.IsLoaded())
            {
                Driver->removeTexture((irr::video::ITexture*)texture.data);
            }
        }
        //===================================//

        //
        //  Irrlicht ICacheToTexture
        class IrrlichtCTT : public Gwk::Renderer::ICacheToTexture
        {
        private:
            Gwk::Renderer::Base *m_renderer;

            irr::video::IVideoDriver* m_Driver;
            std::map<CacheHandle, irr::video::ITexture*> m_TextureCache;

        public:
            IrrlichtCTT() {}

            ~IrrlichtCTT()
            {
                //
                //  Cleanup all cached textures
                for (auto it : m_TextureCache)
                {
                    m_Driver->removeTexture(it.second);
                }
            }

            void SetRenderer(Gwk::Renderer::Base* renderer) { m_renderer = renderer; }

            void SetVideoDriver(irr::video::IVideoDriver* Dvr)
            {
                m_Driver = Dvr;
            }

            void Initialize() {}
            void ShutDown() {}

            //
            //  Create this controls cached texture if it doesn't already exist
            void CreateControlCacheTexture(CacheHandle control, const Point& size)
            {
                if (m_TextureCache.find(control) == m_TextureCache.end())
                {
                    irr::video::ITexture* RTT = m_Driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(size.x, size.y));
                    m_TextureCache.insert(std::pair<CacheHandle, irr::video::ITexture*>(control, RTT));
                }
            }

            //
            //  Setup Irrlicht to cache this control to a texture
            void SetupCacheTexture(CacheHandle control)
            {
                auto it = m_TextureCache.find(control);
                if (it != m_TextureCache.end())
                {
                    m_Driver->setRenderTarget((*it).second);
                }
            }

            //
            //  Revert Irrlicht's render target
            void FinishCacheTexture(CacheHandle control)
            {
                m_Driver->setRenderTarget(NULL);
            }

            //
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
        //===================================//

        //
        //  Irrlicht Renderer
        void Irrlicht::SetIrrlichtDevice(irr::IrrlichtDevice* Device)
        {
            Driver = Device->getVideoDriver();
            m_CTT->SetVideoDriver(Device->getVideoDriver());
            Text = Device->getGUIEnvironment()->getBuiltInFont();
        }

        //
        //  Irrlicht Renderer
        Irrlicht::Irrlicht(ResourceLoader& loader) : Base(loader),
            m_CTT(new IrrlichtCTT())
        {
            m_CTT->SetRenderer(this);
            m_CTT->Initialize();
            DrawColor.set(255, 255, 255, 255);
            ClipRect = irr::core::rect<irr::s32>();
        }

        Irrlicht::~Irrlicht()
        {
            delete m_CTT;
        }

        /*void Irrlicht::Init() {}
        void Irrlicht::Begin() {}
        void Irrlicht::End() {}*/

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
            ClipRect = irr::core::rect<irr::s32>();
        }

        //void Irrlicht::DrawMissingImage(Gwen::Rect pTargetRect) {}

        Gwk::Color Irrlicht::PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y, const Gwk::Color & col_default)
        {
            if (pTexture->IsLoaded())
            {
                irr::video::ITexture* Texture = (irr::video::ITexture*)pTexture->data;

                const irr::u32 pitch = Texture->getPitch();
                const irr::video::ECOLOR_FORMAT format = Texture->getColorFormat();
                static const irr::u32 bytes = irr::video::IImage::getBitsPerPixelFromFormat(format) / 8;

                unsigned char* buffer = (unsigned char*)Texture->lock();
                if (buffer)
                {
                    const irr::video::SColor PixelColor = irr::video::SColor(*(unsigned int*)(buffer + (y * pitch) + (x*bytes)));
                    Texture->unlock();
                    return Gwk::Color(PixelColor.getRed(), PixelColor.getGreen(), PixelColor.getBlue(), PixelColor.getAlpha());
                }
            }
            return col_default;
        }

        ICacheToTexture* Irrlicht::GetCTT()
        {
            return m_CTT;
        }

        void Irrlicht::RenderText(Gwk::Font* pFont, Gwk::Point pos, const Gwk::String & text)
        {
            Translate(pos.x, pos.y);
            Text->draw(text.c_str(),
                irr::core::rect<irr::s32>(pos.x, pos.y, pos.x, pos.y),
                DrawColor,
                false,
                false,
                &ClipRect);
        }

        Gwk::Point Irrlicht::MeasureText(Gwk::Font* pFont, const Gwk::String & text)
        {
            // Get size of string from Irrlicht (hopefully correct for TTF stuff)
            const std::wstring wText(text.begin(), text.end());
            const irr::core::dimension2d<irr::u32> irrDimension = Text->getDimension(wText.c_str());

            return Gwk::Point(irrDimension.Width, irrDimension.Height);;
        }

        void Irrlicht::DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect, float u1, float v1, float u2, float v2)
        {
            if (pTexture->IsLoaded())
            {
                const unsigned int w = pTexture->width;
                const unsigned int h = pTexture->height;

                Translate(pTargetRect);

                Driver->draw2DImage((irr::video::ITexture*)pTexture->data,
                    irr::core::rect<irr::s32>(pTargetRect.x, pTargetRect.y, pTargetRect.x + pTargetRect.w, pTargetRect.y + pTargetRect.h),
                    irr::core::rect<irr::s32>(u1*w, v1*h, u2*w, v2*h),
                    &ClipRect,
                    0,
                    true);
            }
        }

        void Irrlicht::DrawFilledRect(Gwk::Rect rect)
        {
            Translate(rect);
            Driver->draw2DRectangle(DrawColor,
                irr::core::rect<irr::s32>(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h));
        }

        void Irrlicht::DrawLinedRect(Gwk::Rect rect)
        {
            Translate(rect);
            Driver->draw2DRectangleOutline(irr::core::rect<irr::s32>(rect.x, rect.y, rect.x + rect.w, rect.y + rect.h), DrawColor);
        }

        void Irrlicht::DrawPixel(int x, int y)
        {
            Translate(x, y);
            Driver->drawPixel(x, y, DrawColor);
        }

    }
}
