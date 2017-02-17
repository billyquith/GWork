/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#ifndef SFML2_HPP
#define SFML2_HPP

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

            //! Constructor for SFML2 renderer.
            //! \param target - application render target.
            //! \param resDir - path, relative to executable where resources are found.
            SFML2(sf::RenderTarget& target, const String& resDir);
            
            virtual ~SFML2();

            inline void EnsurePrimitiveType(sf::PrimitiveType type)
            {
                if (m_buffer.getPrimitiveType() != type)
                {
                    Flush();
                    m_buffer.setPrimitiveType(type);
                }
            }

            inline void EnsureTexture(const sf::Texture *texture)
            {
                if (m_renderStates.texture != texture)
                {
                    Flush();
                    m_renderStates.texture = texture;
                }
            }

            inline void AddVert(int x, int y, float u = 0.0f, float v = 0.0f)
            {
                if (m_renderStates.texture)
                {
                    u *= m_renderStates.texture->getSize().x;
                    v *= m_renderStates.texture->getSize().y;
                }

                m_buffer.append(sf::Vertex(sf::Vector2f(x, y), m_color, sf::Vector2f(u, v)));
            }

            inline void Flush()
            {
                if (m_buffer.getVertexCount() > 0)
                {
                    m_target.draw(m_buffer, m_renderStates);
                    m_buffer.clear();
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
            virtual void DrawTexturedRect(Gwk::Texture* texture, Gwk::Rect rect, float u1,
                                          float v1, float u2, float v2);

            virtual void RenderText(Gwk::Font* font, Gwk::Point pos,
                                    const Gwk::String& text);
            virtual Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text);
            virtual void        LoadFont(Gwk::Font* font);
            virtual void        FreeFont(Gwk::Font* font);

            virtual void        LoadTexture(Gwk::Texture* texture);
            virtual void        FreeTexture(Gwk::Texture* texture);
            virtual Gwk::Color PixelColor(Gwk::Texture* texture, unsigned int x, unsigned int y,
                                            const Gwk::Color& col_default);

        protected:

            sf::RenderTarget& m_target;
            sf::Color m_color;
            sf::VertexArray m_buffer;
            sf::RenderStates m_renderStates;
            sf::View m_originalView;
            int m_height;
            String m_resourceDir;
        };


    }
}

#endif // SFML2_HPP
