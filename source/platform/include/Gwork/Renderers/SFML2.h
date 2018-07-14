/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#ifndef SFML2_HPP
#define SFML2_HPP

#include <Gwork/BaseRender.h>
#include <Gwork/Platform.h>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <unordered_map>
#include <memory>
#include <vector>
#include <functional>

namespace Gwk
{
    namespace Renderer
    {

        //
        //! Renderer for [SFML2](https://www.sfml-dev.org/).
        //
        class GWK_EXPORT SFML2 : public Gwk::Renderer::Base
        {
            template<typename T>
            using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

        public:

            //! Constructor for SFML2 renderer.
            //! \param loader : ResourceLoader for renderer.
            //! \param target : application render target.
            SFML2(ResourcePaths& paths, sf::RenderTarget& target);

            virtual ~SFML2();

            inline void SetPrimitiveType(sf::PrimitiveType type)
            {
                if (m_buffer.getPrimitiveType() != type)
                {
                    Flush();
                    m_buffer.setPrimitiveType(type);
                }
            }

            inline void SetTexture(const sf::Texture *texture)
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

            struct SFMLTextureData : public Gwk::TextureData
            {
                SFMLTextureData()
                {
                }
                SFMLTextureData(const SFMLTextureData&) = delete;
                SFMLTextureData(SFMLTextureData&& other)
                    : SFMLTextureData()
                {
                    std::swap(width, other.width);
                    std::swap(height, other.height);
                    std::swap(readable, other.readable);

                    texture.swap(other.texture);
                    image.swap(other.image);
                }

                ~SFMLTextureData()
                {
                }

                std::unique_ptr<sf::Texture> texture;
                std::unique_ptr<sf::Image> image;
            };

            struct SFMLFontData
            {
                SFMLFontData()
                {
                }

                SFMLFontData(const SFMLFontData&) = delete;
                SFMLFontData(SFMLFontData&& other)
                    : SFMLFontData()
                {
                    font.swap(other.font);
                }

                ~SFMLFontData()
                {
                }

                std::unique_ptr<sf::Font> font;
            };

            std::unordered_map<Font, SFMLFontData> m_fonts;
            std::unordered_map<Texture, SFMLTextureData> m_textures;
            std::pair<const Font, SFMLFontData>* m_lastFont;
            std::pair<const Texture, SFMLTextureData>* m_lastTexture;

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
