/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_IMAGEPANEL_H
#define GWK_CONTROLS_IMAGEPANEL_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/BaseRender.h>

namespace Gwk {
    namespace Controls {
        
        class GWK_EXPORT ImagePanel : public Controls::Base
        {
        public:

            GWK_CONTROL_INLINE(ImagePanel, Controls::Base)
            {
                SetUV(0, 0, 1, 1);
                SetMouseInputEnabled(false);
                m_drawColor = Colors::White;
                SetStretch(true);
                m_texWidth = 0.0f;
                m_texHeight = 0.0f;
            }

            virtual ~ImagePanel()
            {
                GetSkin()->GetRender()->GetLoader().FreeTexture(m_texture);
            }

            virtual void SetUV(float u1, float v1, float u2, float v2)
            {
                m_uv[0] = u1;
                m_uv[1] = v1;
                m_uv[2] = u2;
                m_uv[3] = v2;
            }

            virtual void SetImage(const String& imageName)
            {
                m_texture.name = imageName;
                IResourceLoader& loader = GetSkin()->GetRender()->GetLoader();
                Texture::Status m_status = loader.LoadTexture(m_texture);
                switch (m_status)
                {
                    case Texture::Status::Loaded:
                    {
                        TextureData texData = loader.GetTextureData(m_texture);
                        m_texWidth = texData.width;
                        m_texHeight = texData.height;
                        break;
                    }
                    default:
                        ;
                }
            }

            virtual String& GetImage()
            {
                return m_texture.name;
            }

            virtual int TextureWidth()
            {
                return m_texWidth;
            }

            virtual int TextureHeight()
            {
                return m_texHeight;
            }

            virtual const String& GetImageName()
            {
                return m_texture.name;
            }

            void Render(Skin::Base* skin) override
            {
                Renderer::Base* render = skin->GetRender();

                render->SetDrawColor(m_drawColor);

                if (m_bStretch)
                {
                    render->DrawTexturedRect(m_texture,
                                             GetRenderBounds(),
                                             m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
                }
                else
                {
                    render->DrawTexturedRect(m_texture,
                                             Gwk::Rect(0, 0, m_texWidth, m_texHeight),
                                             m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
                }
            }

            virtual void SizeToContents()
            {
                SetSize(m_texWidth, m_texHeight);
            }

            virtual void SetDrawColor(Gwk::Color color)
            {
                m_drawColor = color;
            }

            virtual bool FailedToLoad()
            {
                return m_status != Texture::Status::Loaded;
            }

            virtual bool GetStretch()
            {
                return m_bStretch;
            }

            virtual void SetStretch(bool b)
            {
                m_bStretch = b;
            }

        protected:

            Texture m_texture;
            float m_uv[4];
            Gwk::Color m_drawColor;

            bool m_bStretch;
            float m_texWidth, m_texHeight;
            Texture::Status m_status;

        };

    }
}
#endif // ifndef GWK_CONTROLS_IMAGEPANEL_H
