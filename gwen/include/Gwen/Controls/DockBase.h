/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#pragma once
#ifndef GWEN_CONTROLS_DOCKBASE_H
#define GWEN_CONTROLS_DOCKBASE_H

#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Button.h"

namespace Gwen
{
    namespace Controls
    {
        class DockedTabControl;
        class TabControl;

        class GWEN_EXPORT DockBase : public Base
        {
        public:

            GWEN_CONTROL(DockBase, Base);

            virtual void Render(Skin::Base* skin);
            virtual void RenderOver(Skin::Base* skin);
            virtual bool IsEmpty();

            virtual TabControl* GetTabControl();

            virtual DockBase* GetRight()    { return GetChildDock(Docking::Right); }
            virtual DockBase* GetLeft()     { return GetChildDock(Docking::Left); }
            virtual DockBase* GetTop()      { return GetChildDock(Docking::Top); }
            virtual DockBase* GetBottom()   { return GetChildDock(Docking::Bottom); }

            // No action on space (default button action is to press)
            virtual bool OnKeySpace(bool /*bDown*/)     { return false; }

        private:

            // Drag n Drop
            virtual bool DragAndDrop_HandleDrop(Gwen::DragAndDrop::Package* pPackage, int x, int y);
            virtual bool DragAndDrop_CanAcceptPackage(Gwen::DragAndDrop::Package* pPackage);
            virtual void DragAndDrop_HoverEnter(Gwen::DragAndDrop::Package* pPackage, int x, int y);
            virtual void DragAndDrop_HoverLeave(Gwen::DragAndDrop::Package* pPackage);
            virtual void DragAndDrop_Hover(Gwen::DragAndDrop::Package* pPackage, int x, int y);

            virtual void SetupChildDock(Docking::Area iPos);

            virtual void DoRedundancyCheck();
            virtual void DoConsolidateCheck();
            virtual void OnRedundantChildDock(DockBase* pDockBase);

            virtual Docking::Area GetDroppedTabDirection(int x, int y);
            virtual void OnTabRemoved(Gwen::Controls::Base* pControl);

            DockBase*  GetChildDock(Docking::Area iPos);
            DockBase** GetChildDockPtr(Docking::Area iPos);

            DockBase*       m_Left;
            DockBase*       m_Right;
            DockBase*       m_Top;
            DockBase*       m_Bottom;

            // Only CHILD dockpanels have a tabcontrol.
            DockedTabControl*       m_DockedTabControl;

            bool m_bDrawHover;
            bool m_bDropFar;
            Gwen::Rect m_HoverRect;

        };


    }
}
#endif // ifndef GWEN_CONTROLS_DOCKBASE_H
