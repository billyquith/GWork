/*
 *  Gwork
 *  Copyright (c) 2010 Facepunch Studios
 *  Copyright (c) 2013-16 Billy Quith
 *  See license in Gwork.h
 */


#include <Gwork/Gwork.h>
#include <Gwork/Skin.h>
#include <Gwork/Controls/TabStrip.h>
#include <Gwork/Controls/TabControl.h>
#include <Gwork/Controls/Highlight.h>
#include <Gwork/DragAndDrop.h>
#include <Gwork/Utility.h>

using namespace Gwk;
using namespace Gwk::Controls;


GWK_CONTROL_CONSTRUCTOR(TabStrip)
{
    m_TabDragControl = NULL;
    m_bAllowReorder = false;
}

bool TabStrip::DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* /*pPackage*/, int x, int y)
{
    Gwk::Point LocalPos = CanvasPosToLocal(Gwk::Point(x, y));
    TabButton* pButton = gwk_cast<TabButton>(DragAndDrop::SourceControl);
    TabControl* pTabControl = gwk_cast<TabControl>(GetParent());

    if (pTabControl && pButton)
    {
        if (pButton->GetTabControl() != pTabControl)
        {
            // We've moved tab controls!
            pTabControl->AddPage(pButton);
        }
    }

    Base* DroppedOn = GetControlAt(LocalPos.x, LocalPos.y);

    if (DroppedOn)
    {
        Gwk::Point DropPos = DroppedOn->CanvasPosToLocal(Gwk::Point(x, y));
        DragAndDrop::SourceControl->BringNextToControl(DroppedOn, DropPos.x > DroppedOn->Width()/2);
    }
    else
    {
        DragAndDrop::SourceControl->BringToFront();
    }

    return true;
}

bool TabStrip::DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* pPackage)
{
    if (!m_bAllowReorder)
        return false;

    if (pPackage->name == "TabButtonMove")
        return true;

    return false;
}

void TabStrip::Layout(Skin::Base* skin)
{
    Gwk::Point pLargestTab(5, 5);
    int iNum = 0;

    for (Base::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
    {
        TabButton* pButton = gwk_cast<TabButton>(*iter);

        if (!pButton)
            continue;

        pButton->SizeToContents();
        Margin m;
        int iNotFirst = iNum > 0 ? -1 : 0;

        if (m_iDock == Docking::Top)
        {
            m.left = iNotFirst;
            pButton->Dock(Docking::Left);
        }

        if (m_iDock == Docking::Left)
        {
            m.top = iNotFirst;
            pButton->Dock(Docking::Top);
        }

        if (m_iDock == Docking::Right)
        {
            m.top = iNotFirst;
            pButton->Dock(Docking::Top);
        }

        if (m_iDock == Docking::Bottom)
        {
            m.left = iNotFirst;
            pButton->Dock(Docking::Left);
        }

        pLargestTab.x = Gwk::Max(pLargestTab.x, pButton->Width());
        pLargestTab.y = Gwk::Max(pLargestTab.y, pButton->Height());
        pButton->SetMargin(m);
        iNum++;
    }

    if (m_iDock == Docking::Top || m_iDock == Docking::Bottom)
        SetSize(Width(), pLargestTab.y);

    if (m_iDock == Docking::Left || m_iDock == Docking::Right)
        SetSize(pLargestTab.x, Height());

    ParentClass::Layout(skin);
}

void TabStrip::DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* /*pPackage*/, int /*x*/,
                                      int /*y*/)
{
    if (m_TabDragControl)
        Debug::Msg("ERROR! TabStrip::DragAndDrop_HoverEnter\n");

    m_TabDragControl = new ControlsInternal::Highlight(this);
    m_TabDragControl->SetMouseInputEnabled(false);
    m_TabDragControl->SetSize(3, Height());
}

void TabStrip::DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* /*pPackage*/)
{
    delete m_TabDragControl;
    m_TabDragControl = NULL;
}

void TabStrip::DragAndDrop_Hover(Gwk::DragAndDrop::Package* /*pPackage*/, int x, int y)
{
    Gwk::Point LocalPos = CanvasPosToLocal(Gwk::Point(x, y));
    Base* DroppedOn = GetControlAt(LocalPos.x, LocalPos.y);

    if (DroppedOn && DroppedOn != this)
    {
        Gwk::Point DropPos = DroppedOn->CanvasPosToLocal(Gwk::Point(x, y));
        m_TabDragControl->SetBounds(Gwk::Rect(0, 0, 3, Height()));
        m_TabDragControl->BringToFront();
        m_TabDragControl->SetPos(DroppedOn->X()-1, 0);

        if (DropPos.x > DroppedOn->Width()/2)
            m_TabDragControl->MoveBy(DroppedOn->Width()-1, 0);

        m_TabDragControl->Dock(Docking::None);
    }
    else
    {
        m_TabDragControl->Dock(Docking::Left);
        m_TabDragControl->BringToFront();
    }
}

void TabStrip::SetTabPosition(Docking::Area pos)
{
    Dock(pos);

    if (m_iDock == Docking::Top)
        SetPadding(Padding(5, 0, 0, 0));

    if (m_iDock == Docking::Left)
        SetPadding(Padding(0, 5, 0, 0));

    if (m_iDock == Docking::Right)
        SetPadding(Padding(0, 5, 0, 0));

    if (m_iDock == Docking::Bottom)
        SetPadding(Padding(5, 0, 0, 0));

    InvalidateChildren(true);
}
