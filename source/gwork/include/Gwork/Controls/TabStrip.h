/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_TABSTRIP_H
#define GWK_CONTROLS_TABSTRIP_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>
#include <Gwork/Controls/TabButton.h>

namespace Gwk
{
    namespace Controls
    {
        // class TabControl;

        class GWK_EXPORT TabStrip : public Base
        {
        public:

            GWK_CONTROL(TabStrip, Base);

            virtual void Layout(Skin::Base* skin) override;

            virtual bool DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* package, int x, int y) override;
            virtual bool DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* package) override;

            virtual void DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* package, int x, int y) override;
            virtual void DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* package) override;
            virtual void DragAndDrop_Hover(Gwk::DragAndDrop::Package* package, int x, int y) override;

            virtual void SetTabPosition(Docking::Area pos);

            virtual void SetAllowReorder(bool b)
            {
                m_bAllowReorder = b;
            }

            virtual bool AllowsTabReorder()
            {
                return m_bAllowReorder;
            }

            virtual bool ShouldClip() override
            {
                return false;
            }

        private:

            Base*   m_tabDragControl;
            bool m_bAllowReorder;

        };


    }
}
#endif // ifndef GWK_CONTROLS_TABSTRIP_H
