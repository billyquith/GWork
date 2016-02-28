/*
 *  Gwork
 *  Copyright (c) 2011 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
*  See license in Gwork.h
 */

#pragma once
#ifndef GWK_RENDERERS_SFML_H
#define GWK_RENDERERS_SFML_H
#include "Gwork/Gwork.h"
#include "Gwork/BaseRender.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

namespace sf { class RenderTarget; }

namespace Gwk
{
    namespace Renderer
    {
        class SFML : public Gwk::Renderer::Base
        {
        public:

            SFML(sf::RenderTarget& target);
            ~SFML();

            virtual void SetDrawColor(Gwk::Color color);

            virtual void DrawFilledRect(Gwk::Rect rect);

            virtual void LoadFont(Gwk::Font* pFont);
            virtual void FreeFont(Gwk::Font* pFont);
            virtual void RenderText(Gwk::Font* pFont, Gwk::Point pos,
                                    const Gwk::String& text);
            virtual Gwk::Point MeasureText(Gwk::Font* pFont, const Gwk::String& text);

            void StartClip();
            void EndClip();

            void DrawTexturedRect(Gwk::Texture* pTexture, Gwk::Rect pTargetRect, float u1 = 0.0f,
                                  float v1 = 0.0f, float u2 = 1.0f, float v2 = 1.0f);
            void        LoadTexture(Gwk::Texture* pTexture);
            void        FreeTexture(Gwk::Texture* pTexture);
            Gwk::Color PixelColour(Gwk::Texture* pTexture, unsigned int x, unsigned int y,
                                    const Gwk::Color& col_default);

            virtual void DrawLinedRect(Gwk::Rect rect);
            virtual void DrawPixel(int x, int y);

        protected:

            sf::RenderTarget& m_Target;
            sf::Color m_Color;
            sf::RectangleShape m_pixelShape;

        };


    }
}
#endif // ifndef GWK_RENDERERS_SFML_H
