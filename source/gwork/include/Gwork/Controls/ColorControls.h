/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_COLORCONTROLS_H
#define GWK_CONTROLS_COLORCONTROLS_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT ColorLerpBox : public Controls::Base
        {
        public:

            GWK_CONTROL(ColorLerpBox, Controls::Base);
            virtual void Render(Gwk::Skin::Base* skin) override;
            Gwk::Color  GetColorAtPos(int x, int y);
            void         SetColor(Gwk::Color color, bool onlyHue = true);
            virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;
            virtual void OnMouseClickLeft(int x, int y, bool bDown) override;
            Gwk::Color  GetSelectedColor();

            Event::Listener onSelectionChanged;

        protected:

            Gwk::Point cursorPos;
            bool m_bDepressed;
            int m_hue;

        };


        class GWK_EXPORT ColorSlider : public Controls::Base
        {
        public:

            GWK_CONTROL(ColorSlider, Controls::Base);
            virtual void Render(Gwk::Skin::Base* skin) override;
            virtual void OnMouseMoved(int x, int y, int deltaX, int deltaY) override;
            virtual void OnMouseClickLeft(int x, int y, bool bDown) override;
            Gwk::Color  GetSelectedColor();
            Gwk::Color  GetColorAtHeight(int y);
            void         SetColor(Gwk::Color color);

            Event::Listener onSelectionChanged;

        protected:

            int m_selectedDist;
            bool m_bDepressed;

        };


    }

}
#endif // ifndef GWK_CONTROLS_COLORCONTROLS_H
