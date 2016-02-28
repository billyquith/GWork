/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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

            virtual void Render(Skin::Base* skin) override;

            const Gwk::Color& GetColor()
            {
                return m_Color;
            }

            void SetColor(const Gwk::Color& col)
            {
                m_Color = col;
            }

        protected:

            Gwk::Color m_Color;

        };


    }
}
#endif // ifndef GWK_CONTROLS_RECTANGLE_H
