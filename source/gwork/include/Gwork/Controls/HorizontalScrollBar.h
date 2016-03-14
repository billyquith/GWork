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

            virtual int GetBarSize() override
            {
                return m_bar->Width();
            }

            virtual int GetBarPos() override
            {
                return m_bar->X()-Height();
            }

            virtual void SetBarSize(int size) override
            {
                m_bar->SetWidth(size);
            }

            virtual int GetButtonSize() override
            {
                return Height();
            }

            virtual void  ScrollToLeft() override;
            virtual void  ScrollToRight() override;
            virtual void  NudgeLeft(Event::Info);
            virtual void  NudgeRight(Event::Info);
            virtual float GetNudgeAmount() override;

            virtual float CalculateScrolledAmount() override;
            virtual bool  SetScrolledAmount(float amount, bool forceUpdate) override;

            virtual bool IsHorizontal() override
            {
                return true;
            }

        protected:
            
            virtual void OnMouseClickLeft(int x, int y, bool bDown) override;
            virtual void OnBarMoved(Event::Info info) override;

        };


    }
}
#endif // ifndef GWK_CONTROLS_HORIZONTALSCROLLBAR_H
