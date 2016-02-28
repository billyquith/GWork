/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_HORIZONTALSCROLLBAR_H
#define GWK_CONTROLS_HORIZONTALSCROLLBAR_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/Dragger.h>
#include <Gwork/Controls/ScrollBar.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT HorizontalScrollBar : public BaseScrollBar
        {
        public:

            GWK_CONTROL(HorizontalScrollBar, BaseScrollBar);

            virtual void Layout(Skin::Base* skin) override;

            virtual void OnMouseClickLeft(int x, int y, bool bDown) override;
            virtual void OnBarMoved(Controls::Base* control) override;

            virtual int GetBarSize() override
            {
                return m_Bar->Width();
            }

            virtual int GetBarPos() override
            {
                return m_Bar->X()-Height();
            }

            virtual void SetBarSize(int size) override
            {
                m_Bar->SetWidth(size);
            }

            virtual int GetButtonSize() override
            {
                return Height();
            }

            virtual void  ScrollToLeft() override;
            virtual void  ScrollToRight() override;
            virtual void  NudgeLeft(Base* control);
            virtual void  NudgeRight(Base* control);
            virtual float GetNudgeAmount() override;

            virtual float CalculateScrolledAmount() override;
            virtual bool  SetScrolledAmount(float amount, bool forceUpdate) override;

            virtual bool IsHorizontal() override
            {
                return true;
            }

        };


    }
}
#endif // ifndef GWK_CONTROLS_HORIZONTALSCROLLBAR_H
