/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-17 Nick Trout
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TABBUTTON_H
#define GWK_CONTROLS_TABBUTTON_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>

namespace Gwk
{
    namespace Controls
    {
        class TabControl;

        class GWK_EXPORT TabButton : public Button
        {
        public:

            GWK_CONTROL(TabButton, Button);
            void Render(Skin::Base* skin) override;
            void Layout(Skin::Base* skin) override;

            void SetPage(Base* page)
            {
                m_page = page;
            }

            Base* GetPage()
            {
                return m_page;
            }

            void        SetTabControl(TabControl* ctrl);
            TabControl* GetTabControl()
            {
                return m_control;
            }

            bool IsActive()
            {
                return m_page && m_page->Visible();
            }

            bool DragAndDrop_ShouldStartDrag() override;
            void DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* /*package*/,
                                           int /*x*/, int /*y*/) override
            {
                SetHidden(true);
            }

            void DragAndDrop_EndDragging(bool /*bSuccess*/, int /*x*/, int /*y*/) override
            {
                SetHidden(false); SetDepressed(false);
            }

            bool OnKeyLeft(bool bDown) override;
            bool OnKeyRight(bool bDown) override;
            bool OnKeyUp(bool bDown) override;
            bool OnKeyDown(bool bDown) override;

            void UpdateColors() override;

            bool ShouldClip() override
            {
                return false;
            }

        private:

            Base*       m_page;
            TabControl* m_control;

        };

    }
}

#endif // ifndef GWK_CONTROLS_TABBUTTON_H
