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
    m_tabDragControl = NULL;
    m_bAllowReorder = false;
}

bool TabStrip::DragAndDrop_HandleDrop(Gwk::DragAndDrop::Package* /*package*/, int x, int y)
{
    Gwk::Point LocalPos = CanvasPosToLocal(Gwk::Point(x, y));
    TabButton* button = gwk_cast<TabButton>(DragAndDrop::SourceControl);
    TabControl* tabControl = gwk_cast<TabControl>(GetParent());

    if (tabControl && button)
    {
        if (button->GetTabControl() != tabControl)
        {
            // We've moved tab controls!
            tabControl->AddPage(button);
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

bool TabStrip::DragAndDrop_CanAcceptPackage(Gwk::DragAndDrop::Package* package)
{
    if (!m_bAllowReorder)
        return false;

    if (package->name == "TabButtonMove")
        return true;

    return false;
}

void TabStrip::Layout(Skin::Base* skin)
{
    Gwk::Point largestTab(5, 5);
    int iNum = 0;

    for (Base::List::iterator iter = Children.begin(); iter != Children.end(); ++iter)
    {
        TabButton* button = gwk_cast<TabButton>(*iter);

        if (!button)
            continue;

        button->SizeToContents();
        Margin m;
        int iNotFirst = iNum > 0 ? -1 : 0;

        if (m_dock == Docking::Top)
        {
            m.left = iNotFirst;
            button->Dock(Docking::Left);
        }

        if (m_dock == Docking::Left)
        {
            m.top = iNotFirst;
            button->Dock(Docking::Top);
        }

        if (m_dock == Docking::Right)
        {
            m.top = iNotFirst;
            button->Dock(Docking::Top);
        }

        if (m_dock == Docking::Bottom)
        {
            m.left = iNotFirst;
            button->Dock(Docking::Left);
        }

        largestTab.x = Gwk::Max(largestTab.x, button->Width());
        largestTab.y = Gwk::Max(largestTab.y, button->Height());
        button->SetMargin(m);
        iNum++;
    }

    if (m_dock == Docking::Top || m_dock == Docking::Bottom)
        SetSize(Width(), largestTab.y);

    if (m_dock == Docking::Left || m_dock == Docking::Right)
        SetSize(largestTab.x, Height());

    ParentClass::Layout(skin);
}

void TabStrip::DragAndDrop_HoverEnter(Gwk::DragAndDrop::Package* /*package*/, int /*x*/,
                                      int /*y*/)
{
    if (m_tabDragControl)
        Debug::Msg("ERROR! TabStrip::DragAndDrop_HoverEnter\n");

    m_tabDragControl = new ControlsInternal::Highlight(this);
    m_tabDragControl->SetMouseInputEnabled(false);
    m_tabDragControl->SetSize(3, Height());
}

void TabStrip::DragAndDrop_HoverLeave(Gwk::DragAndDrop::Package* /*package*/)
{
    delete m_tabDragControl;
    m_tabDragControl = NULL;
}

void TabStrip::DragAndDrop_Hover(Gwk::DragAndDrop::Package* /*package*/, int x, int y)
{
    Gwk::Point LocalPos = CanvasPosToLocal(Gwk::Point(x, y));
    Base* DroppedOn = GetControlAt(LocalPos.x, LocalPos.y);

    if (DroppedOn && DroppedOn != this)
    {
        Gwk::Point DropPos = DroppedOn->CanvasPosToLocal(Gwk::Point(x, y));
        m_tabDragControl->SetBounds(Gwk::Rect(0, 0, 3, Height()));
        m_tabDragControl->BringToFront();
        m_tabDragControl->SetPos(DroppedOn->X()-1, 0);

        if (DropPos.x > DroppedOn->Width()/2)
            m_tabDragControl->MoveBy(DroppedOn->Width()-1, 0);

        m_tabDragControl->Dock(Docking::None);
    }
    else
    {
        m_tabDragControl->Dock(Docking::Left);
        m_tabDragControl->BringToFront();
    }
}

void TabStrip::SetTabPosition(Docking::Area pos)
{
    Dock(pos);

    if (m_dock == Docking::Top)
        SetPadding(Padding(5, 0, 0, 0));

    if (m_dock == Docking::Left)
        SetPadding(Padding(0, 5, 0, 0));

    if (m_dock == Docking::Right)
        SetPadding(Padding(0, 5, 0, 0));

    if (m_dock == Docking::Bottom)
        SetPadding(Padding(5, 0, 0, 0));

    InvalidateChildren(true);
}
