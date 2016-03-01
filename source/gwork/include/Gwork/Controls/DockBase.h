/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */

#pragma once
#ifndef GWK_CONTROLS_DOCKBASE_H
#define GWK_CONTROLS_DOCKBASE_H

#include <Gwork/Controls/Base.h>
#include <Gwork/Controls/Button.h>

namespace Gwk
{
    namespace Controls
    {
        class DockedTabControl;
        class TabControl;

        class GWK_EXPORT DockBase : public Base
        {
        public:

            GWK_CONTROL(DockBase, Base);

            virtual void Render(Skin::Base* skin) override;
            virtual void RenderOver(Skin::Base* skin) override;
            virtual bool IsEmpty();

            virtual TabControl* GetTabControl();

            virtual DockBase* GetRight()    { return GetChildDock(Docking::Right); }
            virtual DockBase* GetLeft()     { return GetChildDock(Docking::Left); }
            virtual DockBase* GetTop()      { return GetChildDock(Docking::Top); }
            virtual DockBase* GetBottom()   { return GetChildDock(Docking::Bottom); }

            // No action on space (default button action is to press)
            virtual bool OnKeySpace(bool /*bDown*/) override { return false; }

        private:

            // Drag n Drop
            virtual bool DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* package, int x, int y) override;
            virtual bool DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* package) override;
            virtual void DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* package, int x, int y) override;
            virtual void DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* package) override;
            virtual void DragAndDrop_Hover(Gwk::DragAndDrop::Package* package, int x, int y) override;

            virtual void SetupChildDock(Docking::Area iPos);

            virtual void DoRedundancyCheck();
            virtual void DoConsolidateCheck();
            virtual void OnRedundantChildDock(DockBase* dockBase);

            virtual Docking::Area GetDroppedTabDirection(int x, int y);
            virtual void OnTabRemoved(Gwk::Controls::Base* control);

            DockBase*  GetChildDock(Docking::Area iPos);
            DockBase** GetChildDockPtr(Docking::Area iPos);

            DockBase*       m_left;
            DockBase*       m_right;
            DockBase*       m_top;
            DockBase*       m_bottom;

            // Only CHILD dockpanels have a tabcontrol.
            DockedTabControl*       m_dockedTabControl;

            bool m_bDrawHover;
            bool m_bDropFar;
            Gwk::Rect m_hoverRect;

        };


    }
}
#endif // ifndef GWK_CONTROLS_DOCKBASE_H
