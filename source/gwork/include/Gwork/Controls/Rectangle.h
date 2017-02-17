/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_RECTANGLE_H
#define GWK_CONTROLS_RECTANGLE_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Label.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT Rectangle : public Controls::Base
        {
        public:

            GWK_CONTROL(Rectangle, Controls::Base);

            void Render(Skin::Base* skin) override;

            const Gwk::Color& GetColor()
            {
                return m_color;
            }

            void SetColor(const Gwk::Color& col)
            {
                m_color = col;
            }

        protected:

            Gwk::Color m_color;

        };


    }
}
#endif // ifndef GWK_CONTROLS_RECTANGLE_H
