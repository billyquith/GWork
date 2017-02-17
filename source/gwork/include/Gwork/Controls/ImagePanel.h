/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_IMAGEPANEL_H
#define GWK_CONTROLS_IMAGEPANEL_H

#include <Gwork/Gwork.h>
#include <Gwork/Controls/Base.h>
#include <Gwork/BaseRender.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT ImagePanel : public Controls::Base
        {
        public:

            GWK_CONTROL_INLINE(ImagePanel, Controls::Base)
            {
                SetUV(0, 0, 1, 1);
                SetMouseInputEnabled(false);
                m_drawColor = Colors::White;
                SetStretch(true);
            }

            virtual ~ImagePanel()
            {
                GetSkin()->GetRender()->FreeTexture(&m_texture);
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
                GetSkin()->GetRender()->LoadTexture(&m_texture);
            }

            virtual String& GetImage()
            {
                return m_texture.name;
            }

            virtual int TextureWidth()
            {
                return m_texture.width;
            }

            virtual int TextureHeight()
            {
                return m_texture.height;
            }

            virtual const String& GetImageName()
            {
                return m_texture.name;
            }

            void Render(Skin::Base* skin) override
            {
                skin->GetRender()->SetDrawColor(m_drawColor);

                if (m_bStretch)
                {
                    skin->GetRender()->DrawTexturedRect(&m_texture,
                                                        GetRenderBounds(),
                                                        m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
                }
                else
                {
                    skin->GetRender()->DrawTexturedRect(&m_texture,
                                                        Gwk::Rect(0, 0,
                                                                   m_texture.width,m_texture.height),
                                                        m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
                }
            }

            virtual void SizeToContents()
            {
                SetSize(m_texture.width, m_texture.height);
            }

            virtual void SetDrawColor(Gwk::Color color)
            {
                m_drawColor = color;
            }

            virtual bool FailedToLoad()
            {
                return m_texture.failed;
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

        };


    }
}
#endif // ifndef GWK_CONTROLS_IMAGEPANEL_H
