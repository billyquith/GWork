/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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
            void OnBarMoved(Event::Info) override;

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
            virtual void  NudgeUp(Event::Info);
            virtual void  NudgeDown(Event::Info);
            float GetNudgeAmount() override;

            float CalculateScrolledAmount() override;
            bool  SetScrolledAmount(float amount, bool forceUpdate) override;
        };
        
    }
}
#endif // ifndef GWK_CONTROLS_VERTICALSCROLLBAR_H
