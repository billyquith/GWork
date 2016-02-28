/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_COLORPICKER_H
#define GWK_CONTROLS_COLORPICKER_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>


namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT ColorDisplay : public Controls::Base
        {
        public:

            GWK_CONTROL_INLINE(ColorDisplay, Controls::Base)
            {
                SetSize(32, 32);
                m_Color = Color(255, 0, 0, 255);
                m_DrawCheckers = true;
            }

            virtual void Render(Gwk::Skin::Base* skin) override
            {
                skin->DrawColorDisplay(this, m_Color);
            }

            virtual void SetColor(Gwk::Color color)    { m_Color = color; }
            virtual Gwk::Color GetColor()              { return m_Color; }

            virtual void SetRed(int red)                { m_Color.r = red; }
            virtual void SetGreen(int green)            { m_Color.g = green; }
            virtual void SetBlue(int blue)              { m_Color.b = blue; }
            virtual void SetAlpha(int alpha)            { m_Color.a = alpha; }

            virtual void SetDrawCheckers(bool should)   { m_DrawCheckers = should; }

        protected:

            Gwk::Color m_Color;
            bool m_DrawCheckers;
        };


    }
    
    namespace Controls
    {
        class GWK_EXPORT ColorPicker : public Base
        {
        public:

            GWK_CONTROL(ColorPicker, Base);

            virtual void SetAlphaVisible(bool visible);

            virtual void        SetColor(Gwk::Color color);
            virtual Gwk::Color GetColor()      { return m_Color; }

            /// \sect{By name}
            ///     Set colour channel value. Range: 0-255.
            //
            int          GetColorByName(Gwk::String colorName);
            void         SetColorByName(Gwk::String colorName, int colorValue);
            Gwk::String GetColorFromName(Gwk::String name);
            // \}

            /// \sect{Set by value}
            ///     Set colour channel value. Range: 0-255.
            //
            virtual void SetRed(int red)        { m_Color.r = red; }
            virtual void SetGreen(int green)    { m_Color.g = green; }
            virtual void SetBlue(int blue)      { m_Color.b = blue; }
            virtual void SetAlpha(int alpha)    { m_Color.a = alpha; }
            // \}

            Event::Caller onColorChanged;

        protected:

            virtual void Layout(Skin::Base* skin) override;
            virtual void CreateControls();
            virtual void SlidersMoved(Gwk::Controls::Base* control);
            virtual void NumericTyped(Gwk::Controls::Base* control);
            virtual void UpdateControls();
            virtual void UpdateColorControls(Gwk::String name, Gwk::Color col, int sliderVal);
            virtual void CreateColorControl(Gwk::String name, int y);
            
            Gwk::Color m_Color;
        };


    }
}
#endif // ifndef GWK_CONTROLS_COLORPICKER_H
