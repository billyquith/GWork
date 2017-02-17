/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_SCROLLCONTROL_H
#define GWK_CONTROLS_SCROLLCONTROL_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/ScrollBar.h>
#include <Gwork/Controls/VerticalScrollBar.h>
#include <Gwork/Controls/HorizontalScrollBar.h>


namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT ScrollControl : public Base
        {
        public:

            GWK_CONTROL(ScrollControl, Base);

            void Layout(Skin::Base* skin) override;
            void Render(Skin::Base* skin) override;

            virtual void SetScroll(bool h, bool v);
            virtual void SetAutoHideBars(bool should)
            {
                m_bAutoHideBars = should;
            }

            virtual bool CanScrollH() const
            {
                return m_bCanScrollH;
            }

            virtual bool CanScrollV() const
            {
                return m_bCanScrollV;
            }

            void OnChildBoundsChanged(Gwk::Rect oldChildBounds, Base* child) override;
            virtual void UpdateScrollBars();

            virtual void SetVScrollRequired(bool req);
            virtual void SetHScrollRequired(bool req);

            virtual void SetInnerSize(int w, int h);

            virtual void OnVBarMoved(Event::Info);
            virtual void OnHBarMoved(Event::Info);

            bool OnMouseWheeled(int iDelta) override;

            virtual void ScrollToBottom();
            virtual void ScrollToTop();
            virtual void ScrollToLeft();
            virtual void ScrollToRight();

            virtual void Clear();

        protected:

            virtual bool ContentsAreDocked();

            bool m_bCanScrollH;
            bool m_bCanScrollV;

            bool m_bAutoHideBars;

            Controls::BaseScrollBar* m_verticalScrollBar;
            Controls::BaseScrollBar* m_horizontalScrollBar;
        };


    }
}
#endif // ifndef GWK_CONTROLS_SCROLLCONTROL_H
