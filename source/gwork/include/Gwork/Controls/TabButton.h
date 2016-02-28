/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
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
            virtual void Render(Skin::Base* skin) override;
            virtual void Layout(Skin::Base* skin) override;

            void SetPage(Base* page)
            {
                m_Page = page;
            }

            Base* GetPage()
            {
                return m_Page;
            }

            void        SetTabControl(TabControl* ctrl);
            TabControl* GetTabControl()
            {
                return m_Control;
            }

            bool IsActive()
            {
                return m_Page && m_Page->Visible();
            }

            virtual bool DragAndDrop_ShouldStartDrag() override;
            virtual void DragAndDrop_StartDragging(Gwk::DragAndDrop::Package* /*pPackage*/,
                                                   int /*x*/, int /*y*/) override
            {
                SetHidden(true);
            }

            virtual void DragAndDrop_EndDragging(bool /*bSuccess*/, int /*x*/, int /*y*/)
            {
                SetHidden(false); SetDepressed(false);
            }

            virtual bool OnKeyLeft(bool bDown) override;
            virtual bool OnKeyRight(bool bDown) override;
            virtual bool OnKeyUp(bool bDown) override;
            virtual bool OnKeyDown(bool bDown) override;

            virtual void UpdateColours() override;

            virtual bool ShouldClip() override
            {
                return false;
            }

        private:

            Base*       m_Page;
            TabControl* m_Control;

        };


    }
}
#endif // ifndef GWK_CONTROLS_TABBUTTON_H
