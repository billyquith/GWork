/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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
        class GWK_EXPORT Rectangle : public ControlClass<Rectangle, Controls::Base>
        {
        public:

            Rectangle(Gwk::Controls::Base *parent, const Gwk::String &name="");

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
