/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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

            virtual TabButton* AddPage(const String strText, Controls::Base* pPage = NULL);

            virtual void AddPage(TabButton* pButton);
            virtual void RemovePage(TabButton* pButton);

            virtual void OnTabPressed(Controls::Base* control);
            virtual void OnLoseTab(TabButton* pButton);

            virtual int        TabCount(void);
            virtual TabButton* GetTab(int iNum);
            virtual TabButton* GetCurrentButton()
            {
                return m_pCurrentButton;
            }

            virtual TabStrip* GetTabStrip()
            {
                return m_TabStrip;
            }

            virtual void SetTabStripPosition(Docking::Area dockSide);

            virtual bool DoesAllowDrag();

            virtual void SetAllowReorder(bool b)
            {
                GetTabStrip()->SetAllowReorder(b);
            }

            Gwk::Event::Caller onLoseTab;
            Gwk::Event::Caller onAddTab;

        private:

            virtual void PostLayout(Skin::Base* skin) override;
            void         HandleOverflow();

            void ScrollPressLeft(Base* pFrom);
            void ScrollPressRight(Base* pFrom);

            TabStrip*       m_TabStrip;
            TabButton*      m_pCurrentButton;

            ControlsInternal::ScrollBarButton*  m_pScroll[2];
            int m_iScrollOffset;


        };


    }
}
#endif // ifndef GWK_CONTROLS_TABCONTROL_H
