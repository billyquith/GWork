/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_HSVCOLORPICKER_H
#define GWK_CONTROLS_HSVCOLORPICKER_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/ColorControls.h>
#include <Gwork/Controls/ColorPicker.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT HSVColorPicker : public Controls::Base
        {
        public:

            GWK_CONTROL(HSVColorPicker, Controls::Base);

            Gwk::Color GetColor();
            Gwk::Color GetDefaultColor()
            {
                return m_Before->GetColor();
            }

            void SetColor(Gwk::Color color, bool onlyHue = false, bool reset = false);

            void ColorBoxChanged(Gwk::Controls::Base* pControl);
            void ColorSliderChanged(Gwk::Controls::Base* pControl);
            void NumericTyped(Gwk::Controls::Base* control);

            void UpdateControls(Gwk::Color newColor);

            Event::Caller onColorChanged;

        protected:

            ColorLerpBox* m_LerpBox;
            ColorSlider* m_ColorSlider;
            ControlsInternal::ColorDisplay* m_Before;
            ControlsInternal::ColorDisplay* m_After;
        };


    }
}
#endif // ifndef GWK_CONTROLS_HSVCOLORPICKER_H
