/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_DOCKEDTABCONTROL_H
#define GWK_CONTROLS_DOCKEDTABCONTROL_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/TabControl.h>

namespace Gwk
{
    namespace Controls
    {
        class GWK_EXPORT DockedTabControl : public TabControl
        {
        public:

            GWK_CONTROL(DockedTabControl, TabControl);

            void SetShowTitlebar(bool bShow)
            {
                m_titleBar->SetHidden(!bShow);
            }

            void Layout(Skin::Base* skin) override;
            void UpdateTitleBar();

            virtual void DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* package,
                                                   int x, int y) override;
            void DragAndDrop_EndDragging(bool bSuccess, int x, int y) override;

            void MoveTabsTo(DockedTabControl* target);

        private:

            TabTitleBar*    m_titleBar;
            Base*           m_windowControl;

        };


    }
}
#endif // ifndef GWK_CONTROLS_DOCKEDTABCONTROL_H
