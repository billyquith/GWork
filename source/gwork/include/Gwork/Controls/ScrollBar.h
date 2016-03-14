/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SCROLLBAR_H
#define GWK_CONTROLS_SCROLLBAR_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/ScrollBarBar.h>
#include <Gwork/Controls/ScrollBarButton.h>
#include <Gwork/Gwork.h>


#define SCROLL_BUTTON_UP 0
#define SCROLL_BUTTON_LEFT 0
#define SCROLL_BUTTON_DOWN 1
#define SCROLL_BUTTON_RIGHT 1
#define NUDGE_DIST 10

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT BaseScrollBar : public Base
        {
        public:

            GWK_CONTROL(BaseScrollBar, Base);

            virtual void Render(Skin::Base* skin) override;

            virtual void SetBarSize(int size) = 0;
            virtual int  GetBarSize() = 0;
            virtual int  GetBarPos() = 0;

            virtual void ScrollToLeft()     {}
            virtual void ScrollToRight()    {}
            virtual void ScrollToTop()      {}
            virtual void ScrollToBottom()   {}

            virtual float GetNudgeAmount()
            {
                return m_fNudgeAmount/m_fContentSize;
            }

            virtual void SetNudgeAmount(float nudge)
            {
                m_fNudgeAmount = nudge;
            }

            virtual void BarMovedNotification();

            virtual float CalculateScrolledAmount()
            {
                return 0;
            }

            virtual int CalculateBarSize()
            {
                return 0;
            }

            virtual bool SetScrolledAmount(float amount, bool forceUpdate);

            virtual void SetContentSize(float size);
            virtual void SetViewableContentSize(float size);

            virtual int GetButtonSize()
            {
                return 0;
            }

            virtual float GetScrolledAmount()
            {
                return m_fScrolledAmount;
            }

            virtual bool IsHorizontal()
            {
                return false;
            }

            Gwk::Event::Caller onBarMoved;

        protected:

            virtual void OnBarMoved(Event::Info info);
            virtual void OnMouseClickLeft(int /*x*/, int /*y*/, bool /*bDown*/) override {}
            
            ControlsInternal::ScrollBarButton* m_scrollButton[2];
            ControlsInternal::ScrollBarBar* m_bar;

            bool m_bDepressed;
            float m_fScrolledAmount;
            float m_fContentSize;
            float m_fViewableContentSize;
            float m_fNudgeAmount;
        };


    }
}
#endif // ifndef GWK_CONTROLS_SCROLLBAR_H
