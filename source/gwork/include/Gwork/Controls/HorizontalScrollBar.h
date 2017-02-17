/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
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

            void Layout(Skin::Base* skin) override;

            void OnMouseClickLeft(int x, int y, bool bDown) override;
            void OnBarMoved(Event::Info) override;

            int GetBarSize() override
            {
                return m_bar->Width();
            }

            int GetBarPos() override
            {
                return m_bar->X()-Height();
            }

            void SetBarSize(int size) override
            {
                m_bar->SetWidth(size);
            }

            int GetButtonSize() override
            {
                return Height();
            }

            void  ScrollToLeft() override;
            void  ScrollToRight() override;
            virtual void  NudgeLeft(Event::Info);
            virtual void  NudgeRight(Event::Info);
            float GetNudgeAmount() override;

            float CalculateScrolledAmount() override;
            bool  SetScrolledAmount(float amount, bool forceUpdate) override;

            bool IsHorizontal() override
            {
                return true;
            }
        };


    }
}
#endif // ifndef GWK_CONTROLS_HORIZONTALSCROLLBAR_H
