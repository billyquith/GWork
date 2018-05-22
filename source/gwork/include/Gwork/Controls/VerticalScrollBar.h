/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
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

            void Layout(Skin::Base* skin) override;

            void OnMouseClickLeft(int x, int y, bool bDown) override;
            void OnBarMoved(Controls::Base* control) override;

            int GetBarSize() override
            {
                return m_bar->Height();
            }

            int GetBarPos() override
            {
                return m_bar->Y()-Width();
            }

            void SetBarSize(int size) override
            {
                m_bar->SetHeight(size);
            }

            int GetButtonSize() override
            {
                return Width();
            }

            void  ScrollToTop() override;
            void  ScrollToBottom() override;
            virtual void  NudgeUp(Base* control);
            virtual void  NudgeDown(Base* control);
            float GetNudgeAmount() override;

            float CalculateScrolledAmount() override;
            bool  SetScrolledAmount(float amount, bool forceUpdate) override;

        };


    }
}
#endif // ifndef GWK_CONTROLS_VERTICALSCROLLBAR_H
