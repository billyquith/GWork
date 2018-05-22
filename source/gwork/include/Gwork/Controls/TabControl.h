/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-2018 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TABCONTROL_H
#define GWK_CONTROLS_TABCONTROL_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/TabButton.h>
#include <Gwork/Controls/TabStrip.h>
#include <Gwork/Controls/TabTitleBar.h>

namespace Gwk
{
    namespace ControlsInternal
    {
        class ScrollBarButton;
    }

    namespace Controls
    {
        class GWK_EXPORT TabControl : public Base
        {
            GWK_CONTROL(TabControl, Base);

            virtual TabButton* AddPage(const String strText, Controls::Base* page = nullptr);

            virtual void AddPage(TabButton* button);
            virtual void RemovePage(TabButton* button);

            virtual void OnTabPressed(Controls::Base* control);
            virtual void OnLoseTab(TabButton* button);

            virtual int        TabCount(void);
            virtual TabButton* GetTab(int iNum);
            virtual TabButton* GetCurrentButton()
            {
                return m_currentButton;
            }

            virtual TabStrip* GetTabStrip()
            {
                return m_tabStrip;
            }

            virtual void SetTabStripPosition(Position dockSide);

            virtual bool DoesAllowDrag();

            virtual void SetAllowReorder(bool b)
            {
                GetTabStrip()->SetAllowReorder(b);
            }

            Gwk::Event::Caller onLoseTab;
            Gwk::Event::Caller onAddTab;

        private:

            void PostLayout(Skin::Base* skin) override;
            void         HandleOverflow();

            void ScrollPressLeft(Base* from);
            void ScrollPressRight(Base* from);

            TabStrip*       m_tabStrip;
            TabButton*      m_currentButton;

            ControlsInternal::ScrollBarButton*  m_scroll[2];
            int m_scrollOffset;


        };


    }
}
#endif // ifndef GWK_CONTROLS_TABCONTROL_H
