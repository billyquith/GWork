/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#ifndef SFML2_HPP
#define SFML2_HPP

#include <Gwork/Gwork.h>
#include <Gwork/BaseRender.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace Gwk
{
    namespace Renderer
    {
        class SFML2 : public Gwk::Renderer::Base
        {
        public:

            SFML2(sf::RenderTarget& target);
            virtual ~SFML2();

            inline void EnsurePrimitiveType(sf::PrimitiveType type)
            {
                if (m_Buffer.getPrimitiveType() != type)
                {
                    Flush();
                    m_Buffer.setPrimitiveType(type);
                }
            }

            inline void EnsureTexture(const sf::Texture *texture)
            {
                if (m_RenderStates.texture != texture)
                {
                    Flush();
                    m_RenderStates.texture = texture;
                }
            }

            inline void AddVert(int x, int y, float u = 0.0f, float v = 0.0f)
            {
                if (m_RenderStates.texture)
                {
                    u *= m_RenderStates.texture->getSize().x;
                    v *= m_RenderStates.texture->getSize().y;
                }

                m_Buffer.append(sf::Vertex(sf::Vector2f(x, y), m_Color, sf::Vector2f(u, v)));
            }

            inline void Flush()
            {
                if (m_Buffer.getVertexCount() > 0)
                {
                    m_Target.draw(m_Buffer, m_RenderStates);
                    m_Buffer.clear();
                }
            }

            virtual void Begin();
            virtual void End();

            virtual void StartClip();
            virtual void EndClip();

            virtual void SetDrawColor(Gwk::Color color);
            virtual void DrawPixel(int x, int y);
            virtual void DrawLinedRect(Gwk::Rect rect);
            virtual void DrawFilledRect(Gwk::Rect rect);
            virtual void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false);
            virtual void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect rect, float u1,
                                          float v1, float u2, float v2);

            virtual void RenderText(Gwk::Font* pFont, Gwk::Point pos,
                                    const Gwk::String& text);
            virtual Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String& text);
            virtual void        LoadFont(Gwk::Font* font);
            virtual void        FreeFont(Gwk::Font* pFont);

            virtual void        LoadTexture(Gwk::Texture* pTexture);
            virtual void        FreeTexture(Gwk::Texture* pTexture);
            virtual Gwk::Color PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y,
                                            const Gwk::Color& col_default);

        protected:

            sf::RenderTarget& m_Target;
            sf::Color m_Color;
            sf::VertexArray m_Buffer;
            sf::RenderStates m_RenderStates;
            sf::View m_OriginalView;
            int m_Height;
        };


    }
}

#endif // SFML2_HPP
