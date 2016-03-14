/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SLIDER_H
#define GWK_CONTROLS_SLIDER_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/Dragger.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>

namespace Gwk
{
    namespace ControlsInternal
    {
        class GWK_EXPORT SliderBar : public ControlsInternal::Dragger
        {
            GWK_CONTROL(SliderBar, ControlsInternal::Dragger);

            virtual void Render(Skin::Base* skin) override;
            virtual void SetHorizontal(bool b)
            {
                m_bHorizontal = b;
            }

            virtual bool IsHorizontal()
            {
                return m_bHorizontal;
            }

        protected:

            bool m_bHorizontal;
        };


    }

    namespace Controls
    {
        class GWK_EXPORT Slider : public Base
        {
            GWK_CONTROL(Slider, Base);

            virtual void Render(Skin::Base* skin) override = 0;
            virtual void Layout(Skin::Base* skin) override;

            virtual void SetClampToNotches(bool bClamp)
            {
                m_bClampToNotches = bClamp;
            }

            virtual void SetNotchCount(int num)
            {
                m_numNotches = num;
            }

            virtual int GetNotchCount()
            {
                return m_numNotches;
            }

            virtual void  SetRange(float fMin, float fMax);
            virtual float GetFloatValue();
            virtual void  SetFloatValue(float val, bool forceUpdate = true);

            virtual float CalculateValue();
            virtual void  OnMoved(Event::Info info);

            virtual void OnMouseClickLeft(int /*x*/, int /*y*/, bool /*bDown*/) override
            {
            }

            virtual bool OnKeyRight(bool bDown) override
            {
                if (bDown)
                    SetFloatValue(GetFloatValue()+1, true);

                return true;
            }

            virtual bool OnKeyLeft(bool bDown) override
            {
                if (bDown)
                    SetFloatValue(GetFloatValue()-1, true);

                return true;
            }

            virtual bool OnKeyUp(bool bDown) override
            {
                if (bDown)
                    SetFloatValue(GetFloatValue()+1, true);

                return true;
            }

            virtual bool OnKeyDown(bool bDown) override
            {
                if (bDown)
                    SetFloatValue(GetFloatValue()-1, true);

                return true;
            }

            virtual void RenderFocus(Gwk::Skin::Base* skin) override;

            Gwk::Event::Caller onValueChanged;

            virtual float GetMin()
            {
                return m_fMin;
            }

            virtual float GetMax()
            {
                return m_fMax;
            }

        protected:

            virtual void SetValueInternal(float fVal);
            virtual void UpdateBarFromValue() = 0;

            ControlsInternal::SliderBar* m_sliderBar;
            bool m_bClampToNotches;
            int m_numNotches;
            float m_fValue;

            float m_fMin;
            float m_fMax;

        };


    }


}
#endif // ifndef GWK_CONTROLS_SLIDER_H
