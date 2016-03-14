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
                return m_before->GetColor();
            }

            void SetColor(Gwk::Color color, bool onlyHue = false, bool reset = false);

            void ColorBoxChanged(Event::Info info);
            void ColorSliderChanged(Event::Info info);
            void OnNumericTyped(Event::Info info);

            void UpdateControls(Gwk::Color newColor);

            Event::Caller onColorChanged;

        protected:

            ColorLerpBox* m_lerpBox;
            ColorSlider* m_colorSlider;
            ControlsInternal::ColorDisplay* m_before;
            ControlsInternal::ColorDisplay* m_after;
        };


    }
}
#endif // ifndef GWK_CONTROLS_HSVCOLORPICKER_H
