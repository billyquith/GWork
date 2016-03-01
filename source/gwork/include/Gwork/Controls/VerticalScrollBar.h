/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_VERTICALSCROLLBAR_H
#define GWK_CONTROLS_VERTICALSCROLLBAR_H
#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT VerticalScrollBar : public BaseScrollBar
        {
            GWK_CONTROL(VerticalScrollBar, BaseScrollBar);

            virtual void Layout(Skin::Base* skin) override;

            virtual void OnMouseClickLeft(int x, int y, bool bDown) override;
            virtual void OnBarMoved(Controls::Base* control) override;

            virtual int GetBarSize() override
            {
                return m_bar->Height();
            }

            virtual int GetBarPos() override
            {
                return m_bar->Y()-Width();
            }

            virtual void SetBarSize(int size) override
            {
                m_bar->SetHeight(size);
            }

            virtual int GetButtonSize() override
            {
                return Width();
            }

            virtual void  ScrollToTop() override;
            virtual void  ScrollToBottom() override;
            virtual void  NudgeUp(Base* control);
            virtual void  NudgeDown(Base* control);
            virtual float GetNudgeAmount() override;

            virtual float CalculateScrolledAmount() override;
            virtual bool  SetScrolledAmount(float amount, bool forceUpdate) override;

        };


    }
}
#endif // ifndef GWK_CONTROLS_VERTICALSCROLLBAR_H
