/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#ifndef SFML2_HPP
#define SFML2_HPP

#include <Gwork/BaseRender.h>
#include <Gwork/Platform.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace Gwk
{
    namespace Renderer
    {
        //! Default resource loader for SFML2.
        class SFML2ResourceLoader : public ResourceLoader
        {
            ResourcePaths& m_paths;
        public:
            SFML2ResourceLoader(ResourcePaths& paths)
            :   m_paths(paths)
            {}

            Font::Status LoadFont(Font& font) override;
            void FreeFont(Font& font) override;

            Texture::Status LoadTexture(Texture& texture) override;
            void FreeTexture(Texture& texture) override;
        };

        //
        //! Renderer for [SFML2](https://www.sfml-dev.org/).
        //
        class GWK_EXPORT SFML2 : public Gwk::Renderer::Base
        {
        public:

            //! Constructor for SFML2 renderer.
            //! \param loader : ResourceLoader for renderer.
            //! \param target : application render target.
            SFML2(ResourceLoader& loader, sf::RenderTarget& target);

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

            void Begin() override;
            void End() override;

            void StartClip() override;
            void EndClip() override;

            void SetDrawColor(Gwk::Color color) override;
            void DrawPixel(int x, int y) override;
            void DrawLinedRect(Gwk::Rect rect) override;
            void DrawFilledRect(Gwk::Rect rect) override;
            void DrawShavedCornerRect(Gwk::Rect rect, bool bSlight = false) override;
            void DrawTexturedRect(Gwk::Texture* texture,
                                  Gwk::Rect rect,
                                  float u1, float v1, float u2, float v2) override;

            void RenderText(Gwk::Font* font, Gwk::Point pos, const Gwk::String& text) override;
            Gwk::Point MeasureText(Gwk::Font* font, const Gwk::String& text) override;

            Gwk::Color PixelColor(Gwk::Texture* texture,
                                  unsigned int x, unsigned int y,
                                  const Gwk::Color& col_default) override;

        protected:

            sf::RenderTarget& m_target;
            sf::Color m_color;
            sf::VertexArray m_buffer;
            sf::RenderStates m_renderStates;
            sf::View m_originalView;
            int m_height;
        };


    }
}

#endif // SFML2_HPP
